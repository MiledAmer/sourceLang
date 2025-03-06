let wasmInstance;

// Charger le module WebAssembly
async function loadWasm() {
    const response = await fetch("compteur.wasm");
    const bytes = await response.arrayBuffer();
    const wasmModule = await WebAssembly.instantiate(bytes, {});
    
    wasmInstance = wasmModule.instance.exports;

    console.log("WebAssembly chargé !");
}

// Fonction pour mettre à jour l'affichage du compteur
function updateCompteur() {
    const valeur = wasmInstance.get_compteur();
    document.getElementById("compteur").textContent = valeur;
}

// Fonction appelée lorsqu'on clique sur le bouton
function incrementerCompteur() {
    wasmInstance.incrementer_compteur();
    updateCompteur();
}

// Charger WebAssembly au démarrage de la page
loadWasm().then(() => {
    document.getElementById("bouton").addEventListener("click", incrementerCompteur);
});
