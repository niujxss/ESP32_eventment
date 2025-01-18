const button_shuaxin = document.querySelector("#shuaxin");
const temp = document.querySelector("#temp");
const shidu = document.querySelector("#shidu");
const time = document.querySelector("#time");
const date = document.querySelector("#date");


async function fetchdata() {
    try{
        const response = await fetch("http://192.168.31.26:1024/getlastdata");
        if(!response.ok) {
            throw new Error("网络不响应");
        }
        const data = await response.json();

        temp.textContent = data[0]; 
        shidu.textContent = data[1]; 
        time.textContent = data[2]; 
        date.textContent = data[3]; 


    } catch(error) {
        console.error("请求失败：",error);
    }
}


function setAlarm() {

    //console.log("点击按钮");
    fetchdata();
}

button_shuaxin.onclick = () => {
    setAlarm();
};


