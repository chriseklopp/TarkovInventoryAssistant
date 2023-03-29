// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.



function addClientImage(image) {


    var reader = new FileReader();

    // Add new entry to clientImageContainer
    const clientImageContainer = document.getElementById("client-images");
    let imageCount = clientImageContainer.childElementCount;

    const newImg = document.createElement("img");
    newImg.id = "clientImage" + imageCount;


    clientImageContainer.insertBefore(newImg, clientImageContainer.children[0])

    // Add click event handler
    const clientImageElement = document.getElementById(newImg.id);
    clientImageElement.addEventListener("click", function () {


        const children = clientImageElement.parentElement.children;
        for (let i = 0; i < children.length; i++) {
            children[i].classList.remove("highlight-client-image")
        }
        clientImageElement.classList.toggle("highlight-client-image");

        ProcessImageContents(clientImageElement.id);
    });

    reader.onload = function (e) {
        document.getElementById(newImg.id).src = e.target.result;
    }

    reader.readAsDataURL(image);

}


function addExampleImage() {
    fetch("/Resources/exampleImage.png")
        .then(response => response.blob())
        .then(blob => {

            addClientImage(blob);
        });

}

function formAddImage() {
    var image = document.getElementById("uploadClientImage").files[0];
    if (typeof image === "undefined")
        return;
    addClientImage(image);
}


function pasteAddImage(event) {
    var clipboardData = event.clipboardData;
    var items = clipboardData.items;
    if (items[0].type.indexOf("image") !== -1)
        addClientImage(items[0].getAsFile());
}



function removeClientImage() {

}


function currencyCompare(subject, comparee, lessThan = true) {
    var subjectValue = parseFloat(subject.getElementsByTagName('div')[0].innerHTML.replace(/,/g, '').substring(1));
    if (isNaN(subjectValue)) {
        subjectValue = 0
    }
    var compareeValue = parseFloat(comparee.getElementsByTagName('div')[0].innerHTML.replace(/,/g, '').substring(1));
    if (isNaN(compareeValue)) {
        compareeValue = 0
    }

    if (lessThan) {
        return subjectValue < compareeValue;
    }
    else {
        return subjectValue > compareeValue;
    }
    
}

function defaultCompare(subject, comparee, lessThan = true) {
    if (lessThan) {
        return subject.innerHTML.toLowerCase() < comparee.innerHTML.toLowerCase();;
    }
    else {
        return subject.innerHTML.toLowerCase() > comparee.innerHTML.toLowerCase();;
    }
}

function sortTable(n) {
    var table, rows, switching, i, x, y, shouldSwitch, dir, switchcount = 0;
    table = document.getElementsByClassName("sortable")[0];
    var rows = table.getElementsByTagName("tr");

    comparator = defaultCompare;
    //If the column has a special comparison fucntion use that intead.
    var sortType = rows[0].getElementsByTagName("th")[n].getAttribute("data-sort-type");
    if (sortType == "currency") {
        comparator = currencyCompare;
    }

    switching = true;
    dir = "asc";
    while (switching) {
        switching = false;
        rows = table.getElementsByTagName("tr");
        for (i = 1; i < (rows.length - 1); i++) {
            shouldSwitch = false;
            x = rows[i].getElementsByTagName("td")[n];
            y = rows[i + 1].getElementsByTagName("td")[n];

            if (dir == "asc") {
                if (comparator(x,y,false)) {
                    shouldSwitch = true;
                    break;
                }
            }
            else if (dir == "desc") {
                if (comparator(x, y, true)) {
                    shouldSwitch = true;
                    break;
                }
            }

        }
        if (shouldSwitch) {
            rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);
            switching = true;
            switchcount++;
        } else {
            if (switchcount == 0 && dir == "asc") {
                dir = "desc";
                switching = true;
            }
        }
    }
}



