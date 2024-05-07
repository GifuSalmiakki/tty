const form = document.getElementById("form");
var typeInput = document.getElementById("type-input");
var receiveInput = document.getElementById("receive-input");

function submitHandler(e) {
  e.preventDefault();
  
  var typed = typeInput.value;
  const trimmed = typed.trim();

  if (trimmed.length != 0){
    receiveInput.textContent = typed;
  }
  typeInput.value = typeInput.defaultValue;

};

form.addEventListener('submit', submitHandler);
