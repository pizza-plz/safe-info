// ---------------------------------------------
// Configuration required in these 3 lines:
// ---------------------------------------------
const apikey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
const myLat = "42.30260171891152";
const myLong = "-71.17609710203855";
//
//
//
//
//greeting
function greeting() {
    var thetime = new Date();
    var hour = thetime.getHours();
    if(hour < 12) {
        var greeting = "Morning";
    } else if(hour < 17) {
        var greeting = "Afternoon";
    } else if(hour <= 24) {
        var greeting = "Evening";
    }
    document.getElementById("greeting").innerHTML = `Good ${greeting}, Pizza`;
}
// weather api
const baseUrl = "https://api.climacell.co/v3/weather";
const weatherUrl = `${baseUrl}/forecast/daily?lat=${myLat}&lon=${myLong}&unit_system=us&start_time=now&fields=temp&apikey=${apikey}`
// figure out day of the week
function whatDayIsIt(date) {
    var d = new Date(date);
    d = new Date(d.getTime() + d.getTimezoneOffset() * 60000); //stupid timezone correction stuff
    var weekday = new Array(7);
    weekday[0] = "Sunday";
    weekday[1] = "Monday";
    weekday[2] = "Tuesday";
    weekday[3] = "Wednesday";
    weekday[4] = "Thursday";
    weekday[5] = "Friday";
    weekday[6] = "Saturday";
    var result = weekday[d.getDay()];
    return result;
}
// asynchronous fetch
async function getRtWeather(url) {
    let resp = await fetch(url);

    if (!resp.ok) {
        throw new Error(`HTTP error! status: ${resp.status}`);
    } else {
        let results = await resp.json();
        return results;
    }
}
function refresh() {
    // update greeting
    greeting();
    // update weather values
    getRtWeather(weatherUrl)
    .then(results => {
        // log time
        var d = new Date();
        console.log("refreshing data at: ", d);
        // units
        var units = [results].map(() => results[0].temp[0].min.units);
        var tempUnitDivs = document.getElementsByClassName("temp-units");
        for(var i=0; i<tempUnitDivs.length; i++) {
          tempUnitDivs[i].innerHTML = units;
        }
        //day1
        var temp1l = Math.trunc( [results].map(() => results[0].temp[0].min.value) );
        var temp1h = Math.trunc( [results].map(() => results[0].temp[1].max.value) );
        var day1 = [results].map( () => results[0].observation_time.value );
        document.getElementById("day1-temp-low").innerHTML = temp1l;
        document.getElementById("day1-temp-high").innerHTML = temp1h;
        document.getElementById("weather-day1").innerHTML = whatDayIsIt(day1);
        //day2
        var temp2l = Math.trunc( [results].map(() => results[1].temp[0].min.value) );
        var temp2h = Math.trunc( [results].map(() => results[1].temp[1].max.value) );
        var day2 = [results].map( () => results[1].observation_time.value );
        document.getElementById("day2-temp-low").innerHTML = temp2l;
        document.getElementById("day2-temp-high").innerHTML = temp2h;
        document.getElementById("weather-day2").innerHTML = whatDayIsIt(day2);
        //day3
        var temp3l = Math.trunc( [results].map(() => results[2].temp[0].min.value) );
        var temp3h = Math.trunc( [results].map(() => results[2].temp[1].max.value) );
        var day3 = [results].map( () => results[2].observation_time.value );
        document.getElementById("day3-temp-low").innerHTML = temp3l;
        document.getElementById("day3-temp-high").innerHTML = temp3h;
        document.getElementById("weather-day3").innerHTML = whatDayIsIt(day3);
    }).catch(e => {
        console.log("There has been a problem getting data from the API");
    });
}
refresh(); //initial run
setInterval(refresh, 3600000); //refresh once per hour
