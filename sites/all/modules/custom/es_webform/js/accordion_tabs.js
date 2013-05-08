jQuery.noConflict();
jQuery(document).ready(function () {
  //jQuery("p").css("border","red 3px solid");
  jQuery(".product-edit-group-marketspace-tab").next(".product-edit-group-marketspace").hide();
  jQuery(".product-edit-group-marketspace-tab").click(function() {
    jQuery(this).next(".product-edit-group-marketspace").toggle("250");
    jQuery(this).toggleClass('product-edit-group-marketspace-tab-active');
  });
});
