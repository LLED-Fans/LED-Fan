function sliderTrackValue(slider, output) {
    output.innerHTML = slider.value;
    slider.oninput = function() {
        output.innerHTML = this.value;
    }
}

function postRequest(url, data) {
    var form_data = new FormData();
    for (const key in data) {
        form_data.append(key, data[key]);
    }

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState !== 4) {
            return;
        }

        if (this.status !== 200) {
            alert("Failure!");
            return;
        }

        location.reload();
    };
    xhttp.open("POST", url, true);
    xhttp.send(form_data);
}
