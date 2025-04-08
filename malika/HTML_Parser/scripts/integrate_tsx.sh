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
  
  # Vérifier si package.json existe pour confirmer qu'il s'agit d'un projet valide
  if [ ! -f "$PROJECT_PATH\/vite.config.ts" ]; then
    print_error "Aucun fichier package.json trouvé dans $PROJECT_PATH. Est-ce un projet valide?"
    exit 1
  fi
  
  # Vérifier si src existe
  if [ ! -d "$PROJECT_PATH\/src" ]; then
    print_error "Répertoire src non trouvé dans $PROJECT_PATH. Structure de projet invalide."
    exit 1
  fi
  
  print_success "Projet Vite React trouvé: $PROJECT_PATH"
}

# Déplacer le fichier TSX vers le répertoire components
move_tsx_file() {
  print_status "Intégration du fichier TSX dans le projet Vite..."
  
  read -p "Chemin complet du fichier TSX existant: " TSX_PATH
  
  if [ -z "$TSX_PATH" ]; then
    print_error "Aucun chemin fourni pour le fichier TSX"
    exit 1
  fi
  
  # Normaliser le chemin pour Windows
  TSX_PATH=$(normalize_path "$TSX_PATH")
  print_status "Chemin normalisé: $TSX_PATH"
  
  if [ ! -f "$TSX_PATH" ]; then
    print_error "Fichier TSX non trouvé: $TSX_PATH"
    exit 1
  fi
  
  # Obtenir le nom du fichier sans le chemin
  COMPONENT_FILENAME=$(basename "$TSX_PATH")
  COMPONENT_NAME="${COMPONENT_FILENAME%.*}"
  
  # Créer le répertoire components s'il n'existe pas
  mkdir -p "$PROJECT_PATH\/src\/components"
  
  # Chemin de destination
  DEST_PATH="$PROJECT_PATH\/src\/components\/$COMPONENT_FILENAME"
  
  # Copier le fichier vers le répertoire components
  cp "$TSX_PATH" "$DEST_PATH"
  
  if [ $? -eq 0 ]; then
    print_success "Fichier TSX copié vers: $DEST_PATH"
  else
    print_error "Échec de la copie du fichier TSX"
    exit 1
  fi
  
  # Vérifier si le fichier exporté par défaut
  DEFAULT_EXPORT=$(grep -E "export default $COMPONENT_NAME|export default function $COMPONENT_NAME|export default const $COMPONENT_NAME" "$DEST_PATH")
  if [ -z "$DEFAULT_EXPORT" ]; then
    print_warning "Aucun 'export default $COMPONENT_NAME' trouvé dans le fichier."
    print_warning "Assurez-vous que le composant est correctement exporté pour pouvoir l'importer."
  fi
  
  return 0
}

# Mettre à jour App.tsx pour utiliser le composant
update_app_tsx() {
  print_status "Mise à jour du fichier principal pour utiliser le composant $COMPONENT_NAME..."
  
  # Vite utilise soit App.tsx, soit main.tsx comme point d'entrée principal
  if [ -f "$PROJECT_PATH/src/App.tsx" ]; then
    MAIN_FILE="$PROJECT_PATH/src/App.tsx"
  elif [ -f "$PROJECT_PATH/src/App.js" ]; then
    MAIN_FILE="$PROJECT_PATH/src/App.js"
  else
    print_warning "Fichier App.ts/js non trouvé, recherche de main.ts/js..."
    
    if [ -f "$PROJECT_PATH/src/main.ts" ]; then
      MAIN_FILE="$PROJECT_PATH/src/main.ts"
    elif [ -f "$PROJECT_PATH/src/main.js" ]; then
      MAIN_FILE="$PROJECT_PATH/src/main.js"
    else
      print_error "Impossible de trouver le fichier principal (App.tsx/jsx ou main.tsx/jsx)"
      exit 1
    fi
  fi
  
  print_status "Fichier principal identifié: $MAIN_FILE"
  
  # Sauvegarder une copie du fichier principal
  cp "$MAIN_FILE" "${MAIN_FILE}.bak"
  print_status "Copie de sauvegarde créée: ${MAIN_FILE}.bak"
  
  # Vérifier si l'import existe déjà
  IMPORT_EXISTS=$(grep -E "import $COMPONENT_NAME from" "$MAIN_FILE")
  
  if [ -n "$IMPORT_EXISTS" ]; then
    print_warning "Le composant semble déjà être importé dans $MAIN_FILE"
  else
    # Ajouter l'import après le dernier import
    sed -i.tmp "/^import/a\\
import $COMPONENT_NAME from '.\/components\/$COMPONENT_NAME';" "$MAIN_FILE"
    rm -f "${MAIN_FILE}.tmp"
    
    print_status "Import ajouté à $MAIN_FILE"
  fi
  
  # Vérifier si le composant est déjà utilisé
  COMPONENT_USED=$(grep -E "<$COMPONENT_NAME" "$MAIN_FILE")
  
  if [ -n "$COMPONENT_USED" ]; then
    print_warning "Le composant semble déjà être utilisé dans $MAIN_FILE"
  else
    # Déterminer où ajouter le composant
    echo "Où souhaitez-vous ajouter le composant?"
    echo "1: Dans le premier élément div"
    echo "2: Avant le dernier élément fermant (div, fragment, etc.)"
    echo "3: J'indiquerai manuellement un texte à côté duquel l'ajouter"
    read -p "Choisissez une option [1]: " POSITION
    POSITION=${POSITION:-1}
    
    case $POSITION in
      1)
        # Ajouter dans le premier div
        sed -i.tmp "0,/<div/s|<div\([^>]*\)>|<div\\1>\n        <$COMPONENT_NAME />|" "$MAIN_FILE"
        ;;
      2)
        # Ajouter avant la dernière balise fermante
        sed -i.tmp "s|</[^>]*>[ \t]*$|  <$COMPONENT_NAME />\n&|" "$MAIN_FILE"
        ;;
      3)
        # Demander à l'utilisateur d'indiquer un texte repère
        read -p "Entrez un texte ou élément HTML à côté duquel ajouter votre composant: " MARKER
        if [ -z "$MARKER" ]; then
          print_error "Aucun repère fourni"
          exit 1
        fi
        
        # Échapper les caractères spéciaux pour sed
        MARKER_ESCAPED=$(echo "$MARKER" | sed 's/[\/&]/\\&/g')
        
        # Ajouter le composant après le repère
        sed -i.tmp "s|$MARKER_ESCAPED|$MARKER_ESCAPED\n        <$COMPONENT_NAME />|" "$MAIN_FILE"
        ;;
      *)
        print_error "Option non valide"
        exit 1
        ;;
    esac
    rm -f "${MAIN_FILE}.tmp"
    
    if [ $? -eq 0 ]; then
      print_success "Composant ajouté à $MAIN_FILE"
    else
      print_error "Échec de l'ajout du composant à $MAIN_FILE"
      exit 1
    fi
  fi
}

# Menu principal
main() {
  echo "================================================"
  echo "== Intégration d'un composant TSX dans Vite =="
  echo "================================================"
  
  # Détecter le système d'exploitation
  if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    print_status "Système Windows détecté - Adaptation des chemins"
  fi
  
  # Vérifier le projet Vite React
  check_vite_project
  
  # Déplacer le fichier TSX
  move_tsx_file
  
  # Mettre à jour le fichier principal
  update_app_tsx
  
  # Afficher les instructions pour exécuter l'application
  echo ""
  echo "================================================"
  print_success "Intégration terminée!"
  print_status "Votre composant TSX a été intégré à l'application Vite React."
  print_status "Pour exécuter l'application:"
  echo "   cd $PROJECT_PATH"
  echo "   npm run dev"
  echo ""
  print_warning "N'oubliez pas de vérifier que les imports et le JSX sont correctement intégrés"
  print_warning "en examinant le fichier $MAIN_FILE"
  echo "================================================"
}

# Exécuter le programme principal
main