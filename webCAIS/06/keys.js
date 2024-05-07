function onPlay(id) {

    const keys = document.getElementById("keyboard");

    for (var i=0; i < keys.length; ++i){
        console.log(keys[i]);
        if (keys[i].classList.contains("last-played")){
            keys[i].classList.remove("last-played");
        }
    }
    const lastPlayed = document.getElementById(id);
    lastPlayed.classList.add("last-played");
  }
  