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





