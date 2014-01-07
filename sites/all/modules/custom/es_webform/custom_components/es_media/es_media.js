jQuery.noConflict();
/**
 *this responds to the autocompleteSelect event which 
 * is still in autocomplete.js (bootstrap does not override
 * 	this file)
 * 
 * This event still fires twice I could not figure
 * out how to prevent it from firing twice 
 */
jQuery(document).on('autocompleteSelect', function(event, node, itemId) {
	console.log(itemId + "the id and the length " + jQuery("#info-span-" + itemId).length );
	var fid = jQuery("#" + itemId).val();
	var url = "/es_media/user/image/" + fid;
	var imageHtml = "";
	jQuery.ajax({
	  dataType: "json",
	  url: url,
	  success: function (data) {
      jQuery("#" + itemId).after("<span class='ms-image-browser-selection-info'>"
				+ data[fid] + "<input class='ms-image-browser-selection-info-button' type='button' value='Remove' /></span>");
      },
	});
});
jQuery(document).delegate( ".ms-image-browser-selection-info-button", "click",
    function(e) {
    	var imageManager = jQuery(this).closest('.ms-autocomplete-image-manager');
    	jQuery(imageManager).find('.ms-image-browser-selection-info').remove();
	    jQuery(imageManager).find('.ms-image-browser-autocomplete').val('');
	    jQuery(imageManager).find('.ms-image-browser-autocomplete').focus();    	
	    //jQuery(this).closest('.ms-image-browser-selection-info').remove();
	    //jQuery(this).closest('.ms-image-browser-selection-info').remove();
	    // this is the concept behind the below - jQuery('.ms-image-browser-selection-info-button').parent().siblings('.ms-image-browser-autocomplete').css("border","3px blue solid");
	    //jQuery(this).parent().siblings('.ms-image-browser-autocomplete').val('');
	    //jQuery(this).parent().siblings('.ms-image-browser-autocomplete').focus();
    	var inputId = this;
    }
);
/**
 *hide the image browser autocomplete when delete button is clicked 
 */
jQuery('.ms-image-browser-selection-info-button').click( function() {
	jQuery(this).closest('.ms-image-browser-selection-info').hide();
});

/**
 * on browser load check autocompletes for 
 * an image fid and if there is one then
 * create the thumbnail 
 */
jQuery(document).ready(function() {
	jQuery('.ms-image-browser-autocomplete').each(function(index) {
		var fid = jQuery(this).val();
		if(fid != "") {
			var autocomplete = jQuery(this);
			var url = "/es_media/user/image/" + fid;
			var imageHtml = "";
			jQuery.ajax({
			  dataType: "json",
			  url: url,
			  success: function (data) {
		      jQuery('#' + autocomplete.context.id).after("<span class='ms-image-browser-selection-info'>"
						+ data[fid] + "<input class='ms-image-browser-selection-info-button' type='button' value='Remove' /></span>");
					//	jQuery('#' + autocomplete.context.id).css('border', '3px solid red');
		      },
			});
			
		}
	});
});

