<?php

class product_edit extends product {
	  /*
		 * create the attribute like in the product wizard
		 * but add the defaults depending on attribute types
		 */	
	  public function create_attribute_input($attribute, $form_state = null, $component, $product_data) {
	  	$element = parent::create_attribute_input($attribute, $form_state, $component);
			$attribute_type = parent::_convert_attribute_type($attribute->frontend_input);
			switch($attribute_type) {
				case "textfield":
					$element['#default_value'] = $product_data->{$attribute->attribute_code};
					break;
				case "date": //date has to be mm/dd/yyyy for magento to take it
					$element['#default_value'] = $product_data->{$attribute->attribute_code};
					$element['#type'] = 'textfield';
					$element['#attached'] = array(
            'js' => array(drupal_get_path('module', 'es_webform') . '/js/product_edit.js'),
          );
					//this seems stupid also inneficient
					drupal_add_js("http://code.jquery.com/jquery-1.9.1.js", "external");
  				drupal_add_js("http://code.jquery.com/ui/1.10.3/jquery-ui.js", "external");
					$element['#attributes'] = array(
			      'class' => array(
			        'dwg-datepicker',
			      ));
					break;
				case "select":
					if(is_array($product_data->{$attribute->attribute_code})) {
						$element['#default_value'] = $product_data->{$attribute->attribute_code};
					} else { //non core mage select attributes options aren't in array form yet
						$element['#default_value'] = explode(",",$product_data->{$attribute->attribute_code});
					}
					break;
				case "textarea":
					//$element['text'][$attribute->frontend_input]['#default_value'] = $product_data->{$attribute->attribute_code};
					$element = $this->wywiwyg_editor_render($attribute, $component, $product_data);
					
					break;
				default:
			}
			
	  	return $element;
		}

	protected function wywiwyg_editor_render($attribute, $component, $product_data) {
		foreach($attribute->additional_fields as $field) {
  		//check if this is a wysiwyg editor
	    if($field->key == 'is_wysiwyg_enabled' && $field->value == 1) {
	  		/*if(isset($product_data->{$attribute->attribute_code})) {
	  			$default_text = $form_state['storage']['submitted'][$component['cid']]['text'][$attribute->frontend_input]['value'];
	  		}*/
				//create the fieldset for wysiwyg
	  		$form_field = array();
	      //create wysiwyg editor 
	      $form_field = array(
	        '#type' => 'text_format', //http://drupal.org/node/1031160#comment-6256154
	        '#format' => 'content_admin',
	    		'#title' => $attribute->frontend_label[0]->label,
	    		'#required' => (($attribute->is_required == 1) ? TRUE : FALSE),
	    		'#default_value' => isset($product_data->{$attribute->attribute_code}) ? $product_data->{$attribute->attribute_code} : '',
	    		'#description' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
	    		array('#attached'=>array('css' => array(drupal_get_path('module', 'es_webform') . '/css/es_webform_wysiwyg.css'))),
	  		);
				return $form_field;
	  	}
		}
	}
}
