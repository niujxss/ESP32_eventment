let myButton = document.querySelector("button");
let myHeading = document.querySelector("h1");


function setUserName() {
    const Myname = "honey" ;
    myHeading.textContent = `moo is cool,${Myname}`;

}

myButton.onclick = () => {
    setUserName();
};
