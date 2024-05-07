const typeInput = document.getElementById("type-input");
const todoList = document.getElementById("todo");
const form = document.getElementById("form");

function addListItem(text) {
  const listItem = document.createElement("li");
  listItem.textContent = text;
  todoList.appendChild(listItem);

};

function submitHandler(e) {
  e.preventDefault();

  const typed = typeInput.value;
  const trimmed = typed.trim();
  if (trimmed.length != 0){
    addListItem(trimmed);
  }
  typeInput.value = typeInput.defaultValue;
}

function listClickHandler(e) {
  
  const clickedItem = e.target;
  if (clickedItem.tagName == "LI"){

    if (clickedItem.classList.contains("done")){
      todoList.removeChild(clickedItem);
    }
    else {
      clickedItem.classList.add("done");
    }
    
  }
}

form.addEventListener('submit', submitHandler);
todoList.addEventListener('click', listClickHandler);
