jQuery.noConflict();

jQuery(document).ready(function() {
jQuery('.form-autocomplete').change(function(){
      jQuery(this).blur(); // fix for select on click and enter press
      // do your code here
      console.log('hello');
      //my_super_puper_function(this);
});
});
