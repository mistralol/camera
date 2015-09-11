function NumberBoxCheckValue(evt) {
    var charCode = (evt.which) ? evt.which : event.keyCode
    if (charCode == 37 || charCode == 39 || charCode == 8 || charCode == 46)
        return true;
    if (charCode > 31 && ((charCode < 48 || charCode > 57) && (charCode < 96 || charCode > 105)))
        return false;
    return true;
}
