// Fichier à inclure avant le module compilé par Emscripten
var Module = {
    onRuntimeInitialized: function() {
        console.log("Module WebAssembly chargé avec succès!");
    },
    printErr: function(text) {
        console.error(text);
    },
    print: function(text) {
        console.log(text);
        updateCompteurDisplay();
    }
};

// Fonction pour mettre à jour l'affichage du compteur
function updateCompteurDisplay() {
    // Cette fonction sera appelée après l'incrémentation du compteur
    const compteurValue = Module._get_compteur();
    document.getElementById('compteur').textContent = compteurValue;
}

// Fonction appelée par le clic du bouton
function incrementer_compteur() {
    Module._incrementer_compteur();
}