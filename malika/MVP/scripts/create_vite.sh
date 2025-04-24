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

# Vérifier si Node.js est installé
check_node() {
  print_status "Vérification de Node.js..."
  if ! command -v node &> /dev/null; then
    print_error "Node.js n'est pas installé. Veuillez l'installer avant de continuer."
    exit 1
  else
    NODE_VERSION=$(node -v)
    print_success "Node.js est installé (version: $NODE_VERSION)"
  fi
}

# Créer un nouveau projet React avec TypeScript
create_react_app() {
  print_status "Création d'un nouveau projet React avec TypeScript..."
  
  read -p "Nom du projet: " PROJECT_NAME
  
  if [ -z "$PROJECT_NAME" ]; then
    PROJECT_NAME="my-tsx-app"
    print_warning "Aucun nom fourni, utilisation du nom par défaut: $PROJECT_NAME"
  fi
  
  print_status "Création du projet $PROJECT_NAME en cours..."
  npx npm create vite@latest $PROJECT_NAME --template react-ts
  cd $PROJECT_NAME
  npm install
  npm install --save-dev @types/react @types/react-dom
  npm install --save-dev typescript
  
  if [ $? -ne 0 ]; then
    print_error "Échec de l'installation des dépendances npm."
    exit 1
  fi
  
  if [ $? -eq 0 ]; then
    print_success "Projet React avec TypeScript créé avec succès!"
    cd $PROJECT_NAME
    PROJECT_PATH=$(pwd)
    print_status "Chemin du projet: $PROJECT_PATH"
  else
    print_error "Échec de la création du projet React."
    exit 1
  fi
}

# Vérifier le chemin du projet React
check_project() {
  print_status "Vérification du projet React..."
  
  read -p "Chemin du projet React (appuyez sur Entrée pour utiliser le répertoire courant): " PROJECT_PATH
  
  if [ -z "$PROJECT_PATH" ]; then
    PROJECT_PATH="."
    print_warning "Aucun chemin fourni, utilisation du répertoire courant"
  fi
  
  # Vérifier si package.json existe pour confirmer qu'il s'agit d'un projet React
  if [ ! -f "$PROJECT_PATH/package.json" ]; then
    print_error "Aucun fichier package.json trouvé dans $PROJECT_PATH. Est-ce un projet React valide?"
    exit 1
  fi
  
  print_success "Projet React trouvé: $PROJECT_PATH"
}

# Menu principal
main() {
  echo "================================================"
  echo "=== Configuration d'un environnement React/TS ==="
  echo "================================================"
  
  # Vérifier Node.js
  check_node
  
  # Créer un projet React avec TypeScript
  create_react_app
  
}

# Exécuter le programme principal
main