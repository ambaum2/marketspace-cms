<?php
class product_custom_options extends product {
	public static $ajax_option;
	/*
	 * options is an array of objects which have value label like:
	 *     [6] => stdClass Object
        (
            [label] => Multiple Select
            [value] => multiple
        )
	 * select type for future can specify whether select, checkbox, radio etc
	 */
	public function renderOptionTypeSelect($options, $element_id, $title, $description = "", $filter_values = array(), $select_type = "select", $defaults = array()) {
		$element[$element_id] = array(
	    '#type' => $select_type,
	    '#title' => t($title),
  	);
		$all_options = getCustomOptionType($options);

		$element[$element_id]['#options'] = $all_options;
	  $element[$element_id]['#default_value'] = $defaults;
	  $element[$element_id]['#description'] = t($description);
		return $element;
	}
	public function getCustomOptionType($options,$filter_values = array()) {
		$all_options = array('none' => "Select a Type");	
		foreach($options as $key=>$value) {
			if(!(in_array($value->value, $filter_values))) {
				$all_options[$value->value] = $value->label;
			}
		}
		return $all_options;
	}
	/*
	 * create a template with option name, type, 
	 * required and order
	 * 
	 * $custom_option (Object) is the custom option data - for pre
	 * populating
	 */
	public function renderOptionInfoForm($options, $element_id, $form_state, $custom_option = null, $filter_values = array()) {
		$product_custom_options_templates = new product_custom_options_templates;
		$all_options = $this->getCustomOptionType($options,$filter_values);
    //if custom option already in magento get the id
    $option_id = false;
    if(isset($custom_option->id) || isset($form_state['values']['custom_options'][$element_id]['magento_option_id'])) {
      (isset($custom_option->id)) ? $option_id = $custom_option->id : 
        $option_id = $form_state['values']['custom_options'][$element_id]['magento_option_id'];
    }
		$element = $product_custom_options_templates->custom_option_type_template($element_id, $option_id, $custom_option);
		//watchdog('product id options renderfx','type: %value_type id: %id <pre>%options</pre>',array('%options'=>print_r($form_state,true),'%id'=>$element_id, '%value_type'=>$option_value_type),WATCHDOG_NOTICE);
		/*$element = array(
			'#type' => 'fieldset',
			'#title' => "Option",
			'#attributes' => array(
				'class' => array(
					'es-custom-option-type',
				),
			),
			//'#disabled' => ($option_id) ? TRUE : FALSE,
		);
		$element['title'] = array(
			'#type' => 'textfield',
			'#title' => 'Title',
			'#size' => 20,
			'#maxlength' => 128,
			'#default_value' => isset($custom_option->title) ? $custom_option->title : '',
		);		
		$element['type'] = array(
			'#type' => 'select',
			'#title' => 'Input Type',
			'#options' => $all_options,
			'#disabled' => ($option_id) ? TRUE : FALSE,
      '#ajax' => array(
        'callback' => 'getCustomOptionsTypeTemplate',//'getOptionsTypeTemplate',
        'wrapper' => 'custom-options-option-value-' . $element_id,
      ),
      '#prefix' => '<!--<attributes><id>'. $element_id . '</id></attributes>-->',
		);*/
    if(isset($custom_option->type)) {
      $element['type']['#default_value'] = $custom_option->type;
    } elseif(isset($form_state['values']['custom_options'][$element_id]['type'])) {
      $element['type']['#default_value'] = $form_state['values']['custom_options'][$element_id]['type'];
    }
    
		/*$element['required'] = array(
			'#type' => 'select',
			'#title' => 'Is Required',
			'#options' => array(
				0=>t('No'), 
				1=>t('Yes'),
			),
			'#default_value' => isset($custom_option->is_require) ? $custom_option->is_require : '',
		);
		$element['order'] = array(
			'#type' => 'textfield',
			'#title' => 'Sort Order',
			'#size' => 5,
			'#maxlength' => 3,
			'#default_value' => isset($custom_option->sort_order) ? $custom_option->sort_order : '',
		);		
		
		$element['values'] = array(
			'#type' => 'fieldset',
			'#title' => "Option Values",
			'#prefix' => "<div id='custom-options-option-value-" . $element_id . "'>",
			'#attributes' => array(
				'class' => array(
					'es-custom-option-type',
				),
			),
			'#suffix' => "</div>",
		);*/
		/*
		 * this is how we distinguish between new
		 * and existing custom options when form
		 * has been submitted
		 */
  	$element['magento_option_id'] = array(
			'#type'=>'hidden', 
			'#value' => ($option_id) ? $option_id : '',
  	);
		 if(isset($form_state['values']['custom_options'][$element_id]['type']) || isset($custom_option->additional_fields)) {
			isset($form_state['values']['custom_options'][$element_id]['type']) ? $option_value_type = $form_state['values']['custom_options'][$element_id]['type'] :
       $option_value_type = $custom_option->type;
       
			//watchdog('product id options renderfx','type: %value_type id: %id <pre>%options</pre>',array('%options'=>print_r($form_state,true),'%id'=>$element_id, '%value_type'=>$option_value_type),WATCHDOG_NOTICE);
			if($option_value_type == "field" || $option_value_type == "area") {
  	    $element['values']['price'] = array(
  	    	'#type' => 'textfield',
  	      '#title' => t('Price'),
  	      '#default_value' => isset($custom_option->additional_fields[0]->price) ? $custom_option->additional_fields[0]->price : '',
  	      '#size' => 5,
  	      );
        $element['values']['price_type'] = array(
          '#type' => 'select',
          '#title' => 'Price Type',
          '#options' => array(
            'fixed'=>t('Price'),
            'percent'=>t('Percent'),
          ),
          '#default_value' => isset($custom_option->additional_fields[0]->price_type) ? $custom_option->additional_fields[0]->price_type : '',
        );  
        $element['values']['sku'] = array(
          '#type' => 'textfield',
          '#title' => t('SKU'),
          '#default_value' => isset($custom_option->additional_fields[0]->sku) ? $custom_option->additional_fields[0]->sku : '',
          '#size' => 5,
        );
        $element['values']['max_characters'] = array(
          '#type' => 'textfield',
          '#title' => t('Max Characters'),
          '#default_value' => isset($custom_option->additional_fields[0]->max_characters) ? $custom_option->additional_fields[0]->max_characters : '',
          '#size' => 5,
          );     
			} elseif($option_value_type=="drop_down") {
  			  $element['values']['values_type'] = array(
  	    	'#type' => 'textfield',
  	      '#title' => t('Who dropdown'),
  	      );
			} elseif($option_value_type == "date" || $option_value_type == "time" ||
          $option_value_type == "date_time") {
          $element['values']['price'] = array(
            '#type' => 'textfield',
            '#title' => t('Price'),
            '#default_value' => isset($custom_option->additional_fields[0]->price) ? $custom_option->additional_fields[0]->price : '',
            '#size' => 5,
            );
          $element['values']['price_type'] = array(
            '#type' => 'select',
            '#title' => 'Price Type',
            '#options' => array(
              'fixed'=>t('Price'),
              'percent'=>t('Percent'),
            ),
            '#default_value' => isset($custom_option->additional_fields[0]->price_type) ? $custom_option->additional_fields[0]->price_type : '',
            '#size' => 5,
          );  
          $element['values']['sku'] = array(
            '#type' => 'textfield',
            '#title' => t('SKU'),
            '#default_value' => isset($custom_option->additional_fields[0]->sku) ? $custom_option->additional_fields[0]->sku : '',
            '#size' => 5,
          );			
      } else {
	      $element['values']['values_type'] = array();
			}
		} 
    if($option_id) {
      $element['delete'] = array(
        '#type' => 'checkbox',
        '#title' => 'Delete',
        '#description' => t('Existing custom options must be checked if you want to delete them. Click Save Product below to confirm'),
      );      
    } else {
      $element['delete'] = array(
        '#type' => 'submit',
        '#name' => 'delete_option_' . $element_id,
        '#value' => t('Delete Custom Option'),
        '#submit' => array('add_custom_option'),
        '#prefix' => '<!--<attributes><id>'. $element_id . '</id></attributes>-->',
      );
    }
		return $element;
	}

	/*
	 * create the custom option value templates
	 * one for text, select, and date types
	 * they are rendered all at once but hidden
	 * @TODO - file type
	 * 
	/* 
 * hardcode custom options for now
 * no reason to put them in a table or pull
 * them in because they don't change without
 * a custom magento module
 */
	public function getAllCustomOptions() {	
		$options_array = array(
	  	array('label'=>'Field','value' => 'field'),
			array('label' => 'Area','value' => 'area'),
			array('label' => 'File','value'	=> 'file'),
			//array('label' => 'Drop-down','value' => 'drop_down'),
			//array('label' => 'Radio Buttons','value' => 'radio'),
			//array('label' => 'Checkbox','value' => 'checkbox'),
			//array('label' => 'Multiple Select','value' => 'multiple'),
			array('label' => 'Date','value' => 'date'),
			array('label' => 'Date & Time','value' => 'date_time'),
			array('label' => 'Time','value' => 'time'),
		);
		$options = new stdClass;
		$i = 0;
		foreach($options_array as $key=>$value) {
			$options->$key->label = $value['label'];
			$options->$key->value = $value['value'];
		}
		return $options;
	}
}
