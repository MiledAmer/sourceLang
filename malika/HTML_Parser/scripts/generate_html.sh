#!/bin/bash
# Définition des couleurs pour le terminal
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Compiler le programme C
gcc -o html_generator code.c

# Vérifier si la compilation a réussi
if [ $? -eq 0 ]; then
  echo -e "${GREEN}[SUCCÈS]${NC} $1"

  # Exécuter le binaire avec un argument (nom du fichier HTML)
  ./html_generator index.html
else
  echo -e "${RED}[ERREUR]${NC} $1"
fi
