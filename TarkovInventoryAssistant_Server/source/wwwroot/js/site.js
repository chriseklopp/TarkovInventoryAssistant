// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.


function addClientImage() {

    var image = document.getElementById("uploadClientImage").files[0];

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
        clientImageElement.classList.toggle("highlight");
        ProcessImageContents(clientImageElement.id);
    });

    reader.onload = function (e) {
        document.getElementById(newImg.id).src = e.target.result;
    }

    reader.readAsDataURL(image);

}



