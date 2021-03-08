var mobile_menu_view = false;


/* Toggle the mobile view hamburger menu */
function toggleMenu() {
    if(mobile_menu_view) {
        document.getElementById("hm_icon").style.display = "block";
        document.getElementById("links").style.display = "none";
        mobile_menu_view = false;
    }

    else {
        document.getElementById("hm_icon").style.display = "none";
        document.getElementById("links").style.display = "flex";
        mobile_menu_view = true;
    }
}
