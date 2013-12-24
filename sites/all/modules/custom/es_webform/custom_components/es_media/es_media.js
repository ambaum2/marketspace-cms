jQuery.noConflict();
jQuery('.form-autocomplete').live('autocompleteSelect', function() {
	var autocomplete = jQuery(this).next('.ms-image-browser-autocomplete');
	var fid = jQuery("#" + autocomplete.context.id).val();
	var url = "/es_media/user/image/" + fid;
	var imageHtml = "";
	jQuery.ajax({
	  dataType: "json",
	  url: url,
	  success: function (data) {
      jQuery("#" + autocomplete.context.id).after("<span class='ms-image-browser-selection-info'>"
				+ data[fid] + "<input class='ms-image-browser-selection-info-button' type='button' value='Remove' /></span>");
      },
	});

});
jQuery(document).delegate( ".ms-image-browser-selection-info-button", "click",
    function(e) {
	    jQuery(this).closest('.ms-image-browser-selection-info').hide();
	    // this is the concept behind the below - jQuery('.ms-image-browser-selection-info-button').parent().siblings('.ms-image-browser-autocomplete').css("border","3px blue solid");
	    jQuery(this).parent().siblings('.ms-image-browser-autocomplete').val('');
	    jQuery(this).parent().siblings('.ms-image-browser-autocomplete').focus();
    var inputId = this;
        console.log( inputId );
    }
);
/**
 *hide the image browser autocomplete when delete button is clicked 
 */
jQuery('.ms-image-browser-selection-info-button').click( function() {
	console.log("clicked");
	jQuery(this).closest('.ms-image-browser-selection-info').hide();
});

