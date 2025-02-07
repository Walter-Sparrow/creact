// This script will run after main.js is loaded, so we can listen for when WASM is ready.
Module.onRuntimeInitialized = () => {
  console.log("WASM runtime initialized.");

  const button = document.getElementById("callAdd");
  button.addEventListener("click", () => {
    // Call the C function 'add' via ccall.
    const sum = Module.ccall(
      "add", // name of C function
      "number", // return type
      ["number", "number"], // arg types
      [5, 6] // arg values
    );

    Module.ccall("callAdd", null, []);

    document.getElementById("result").textContent = `Result: ${sum}`;
  });
};
