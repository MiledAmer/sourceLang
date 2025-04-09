#!/bin/bash

# Définition des couleurs pour le terminal
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Fonction pour afficher les messages de statut
print_status() {
  echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
  echo -e "${GREEN}[SUCCÈS]${NC} $1"
}

print_warning() {
  echo -e "${YELLOW}[ATTENTION]${NC} $1"
}

print_error() {
  echo -e "${RED}[ERREUR]${NC} $1"
}

# Fonction pour normaliser les chemins Windows
normalize_path() {
  local path="$1"
  
  # Remplacer les backslashes par des slashes pour les chemins Windows
  if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    # Convertir chemins Windows (C:\...) en chemins compatibles avec bash (/c/...)
    path=$(echo "$path" | sed 's/\\/\//g')
    
    # Pour les chemins absolus Windows (commençant par une lettre de lecteur)
    if [[ "$path" =~ ^[A-Za-z]: ]]; then
      # Extraire la lettre du lecteur et le reste du chemin
      drive_letter=$(echo "$path" | cut -c1)
      remaining_path=$(echo "$path" | cut -c3-)
      path="/$drive_letter$remaining_path"
    fi
  fi
  
  echo "$path"
}

# Vérifier le chemin du projet Vite React
check_vite_project() {
  print_status "Vérification du projet Vite React..."
  
  read -p "Chemin du projet Vite React (appuyez sur Entrée pour utiliser le répertoire courant): " PROJECT_PATH
  
  if [ -z "$PROJECT_PATH" ]; then
    PROJECT_PATH="."
    print_warning "Aucun chemin fourni, utilisation du répertoire courant"
  else
    PROJECT_PATH=$(normalize_path "$PROJECT_PATH")
  fi
  
  # Vérifier si vite.config.ts/js existe pour confirmer qu'il s'agit d'un projet valide
  if [ ! -f "$PROJECT_PATH/vite.config.ts" ] && [ ! -f "$PROJECT_PATH/package.json" ]; then
    print_error "Aucun fichier vite.config.ts/js trouvé dans $PROJECT_PATH. Est-ce un projet Vite valide?"
    exit 1
  fi
  
  # Vérifier si le projet a une structure valide
  if [ ! -d "$PROJECT_PATH/public" ] && [ ! -d "$PROJECT_PATH/src" ]; then
    print_error "Répertoires public ou src non trouvés dans $PROJECT_PATH. Structure de projet invalide."
    exit 1
  fi
  
  print_success "Projet Vite React trouvé: $PROJECT_PATH"
}

# Déplacer le fichier HTML vers le bon emplacement
move_html_file() {
  print_status "Intégration du fichier index.html dans le projet Vite..."
  
  read -p "Chemin complet du fichier index.html existant: " HTML_PATH
  
  if [ -z "$HTML_PATH" ]; then
    print_error "Aucun chemin fourni pour le fichier HTML"
    exit 1
  fi
  
  # Normaliser le chemin pour Windows
  HTML_PATH=$(normalize_path "$HTML_PATH")
  print_status "Chemin normalisé: $HTML_PATH"
  
  if [ ! -f "$HTML_PATH" ]; then
    print_error "Fichier HTML non trouvé: $HTML_PATH"
    exit 1
  fi
  
  # Déterminer l'emplacement de destination
  # Dans Vite, index.html est généralement à la racine du projet
  
  # Vérifier si un index.html existe déjà dans le projet
  if [ -f "$PROJECT_PATH/index.html" ]; then
    print_warning "Un fichier index.html existe déjà dans le projet."
    read -p "Voulez-vous le remplacer? (o/N): " REPLACE
    
    if [[ "$REPLACE" -eq "o" && "$REPLACE" -eq "O" ]]; then
      print_status "Création d'une copie de sauvegarde..."
      cp "$PROJECT_PATH/index.html" "$PROJECT_PATH/index.html.bak"
      print_success "Sauvegarde créée: $PROJECT_PATH/index.html.bak"
    else
      print_status "Opération annulée"
      exit 0
    fi
  fi
  
  # Copier le fichier HTML vers la racine du projet
  cp "$HTML_PATH" "$PROJECT_PATH/index.html"
  
  if [ $? -eq 0 ]; then
    print_success "Fichier HTML copié vers: $PROJECT_PATH/index.html"
  else
    print_error "Échec de la copie du fichier HTML"
    exit 1
  fi
  
  return 0
}

# Vérifier la structure du fichier HTML
check_html_structure() {
  print_status "Vérification de la structure du fichier HTML..."
  
  # Vérifier si le fichier contient un élément root pour l'application React
  ROOT_ELEMENT=$(grep -E "id=['\"]root['\"]" "$PROJECT_PATH/index.html")
  
  if [ -z "$ROOT_ELEMENT" ]; then
    print_warning "Aucun élément avec id=\"root\" trouvé dans le fichier HTML."
    print_warning "Vite React nécessite généralement un élément (comme <div id=\"root\"></div>) pour monter l'application."
    
    read -p "Voulez-vous ajouter automatiquement un élément root? (O/n): " ADD_ROOT
    
    if [[ "$ADD_ROOT" != "n" && "$ADD_ROOT" != "N" ]]; then
      # Ajouter un div#root avant la fermeture du body
      sed -i.tmp "s|</body>|  <div id=\"root\"></div>\n</body>|" "$PROJECT_PATH/index.html"
      rm -f "${PROJECT_PATH}/index.html.tmp"
      print_success "Élément <div id=\"root\"></div> ajouté au fichier HTML."
    fi
  fi
  
  # Vérifier si le script principal est correctement référencé
  SCRIPT_TAG=$(grep -E "<script[^>]*src=['\"][^'\"]*main\.(js|ts|jsx|tsx)['\"]" "$PROJECT_PATH/index.html")
  
  if [ -z "$SCRIPT_TAG" ]; then
    print_warning "Aucune référence au script principal trouvée dans le fichier HTML."
    print_warning "Dans Vite, vous avez généralement besoin d'une balise comme:"
    print_warning "<script type=\"module\" src=\"/src/main.tsx\"></script>"
    
    read -p "Voulez-vous ajouter automatiquement une référence au script? (O/n): " ADD_SCRIPT
    
    if [[ "$ADD_SCRIPT" != "n" && "$ADD_SCRIPT" != "N" ]]; then
      # Déterminer le fichier principal
      MAIN_FILE=""
      for ext in ts tsx js jsx; do
        if [ -f "$PROJECT_PATH/src/main.$ext" ]; then
          MAIN_FILE="main.$ext"
          break
        fi
      done
      
      if [ -z "$MAIN_FILE" ]; then
        print_warning "Fichier main.ts/js/tsx/jsx non trouvé. Utilisation de main.tsx par défaut."
        MAIN_FILE="main.tsx"
      fi
      
      # Ajouter le script avant la fermeture du body
      sed -i.tmp "s|</body>|  <script type=\"module\" src=\"/src/$MAIN_FILE\"></script>\n</body>|" "$PROJECT_PATH/index.html"
      rm -f "${PROJECT_PATH}/index.html.tmp"
      print_success "Référence au script principal ajoutée au fichier HTML."
    fi
  fi
}

# Menu principal
main() {
  echo "================================================"
  echo "== Intégration d'un fichier index.html dans Vite =="
  echo "================================================"
  
  # Détecter le système d'exploitation
  if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    print_status "Système Windows détecté - Adaptation des chemins"
  fi
  
  # Vérifier le projet Vite React
  check_vite_project 
  
  # Déplacer le fichier HTML
  move_html_file
  
  # Vérifier et ajuster la structure du fichier HTML
  check_html_structure
  
  # Afficher les instructions pour exécuter l'application
  echo ""
  echo "================================================"
  print_success "Intégration terminée!"
  print_status "Votre fichier index.html a été intégré à l'application Vite React."
  print_status "Pour exécuter l'application:"
  echo "   cd $PROJECT_PATH"
  echo "   npm run dev"
  echo ""
  print_warning "N'oubliez pas de vérifier que le fichier index.html est correctement configuré"
  print_warning "et contient les références nécessaires aux scripts et styles de votre application."
  echo "================================================"
}

# Exécuter le programme principal
main