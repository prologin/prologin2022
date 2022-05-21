const LOGO_URL = "/static/img/logo.png";


let placeLogo = () => {
  let imgTag = document.createElement("img");
  imgTag.src = LOGO_URL;
  imgTag.style.width = "100%";
  imgTag.style.padding = "20px 10px";
  document.querySelector('[role="complementary"]').prepend(imgTag);
};

document.addEventListener("DOMContentLoaded", function() {


  // Adds the finale logo to the website
  placeLogo();


})
