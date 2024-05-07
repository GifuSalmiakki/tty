import logo from './pets.jpg';
import './App.css';
import React, { useState } from 'react';

function App() {

  const [name, setName] = useState("")
  const [animal, setAnimal] = useState("")

  var messages = [
    `${name}, you are a very wonderful ${animal} and I like you very much! :)`,
    `${name}, you are the most beautiful ${animal}!`,
    `${name}, you are such a smart ${animal}!`,
    `${name}, you're my favourite ${animal}!`,
    `${name}, amongst all ${animal}s you are undoubtedly the best!`

  ];

  const handleSubmit = () => {
    var message = messages[Math.floor(Math.random()*messages.length)];
    alert(message)
  }
  
  return (
    <div className="App">
      <header className="App-header">

        <h2> Exercise 2.3</h2>
        <p> Hello! Please enter your pet's info below </p>
      
        <img src={logo} alt="pet dogs and cats" />

          <form onSubmit={handleSubmit}>

            <label>Enter the pet's name:
            <input type="text"
            value={name}
            onChange={(e) => setName(e.target.value)}
            />
            </label>

            <br></br>

            <label>What kind of an animal is your pet: 
            <input type="text"
            value={animal}
            onChange={(e) => setAnimal(e.target.value)}
            />
            </label>

            <br></br>

            <button class="clickMe" onclick="displayMessage()">Submit</button>

          </form>
        
      </header>
    </div>
  );

}

export default App;
