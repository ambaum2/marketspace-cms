jQuery.noConflict();

jQuery(document).ready(function() {
	jQuery(".child_category_select").click(function() {
		the_parent = jQuery(this).parents(".parent_category");
		jQuery(this).toggleClass("child_category_expand");
		jQuery(the_parent).next(".child_categories_container").toggleClass("child_categories_hide");
	});
});
