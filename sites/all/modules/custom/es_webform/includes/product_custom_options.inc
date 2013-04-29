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
		$all_options = array();	
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
	 */
	public function renderOptionInfoForm($options, $element_id, $form_state, $filter_values = array()) {
		$all_options = $this->getCustomOptionType($options,$filter_values);
		$element[$element_id] = array(
			'#type' => 'fieldset',
			'#title' => "Option",
			'#attributes' => array(
				'class' => array(
					'es-custom-option-type',
				),
			),
		);
		$element[$element_id]['title'] = array(
			'#type' => 'textfield',
			'#title' => 'Title',
			'#size' => 20,
			'#maxlength' => 128,
		);		
		$element[$element_id]['type'] = array(
			'#type' => 'select',
			'#title' => 'Input Type',
			'#options' => $all_options,
      '#ajax' => array(
        'callback' => 'getOptionsTypeTemplate',
        'wrapper' => 'custom-options-option-value-' . $element_id,
      ),
      '#prefix' => '<!--<attributes><id>'. $element_id . '</id></attributes>-->',
		);

		$element[$element_id]['required'] = array(
			'#type' => 'select',
			'#title' => 'Is Required',
			'#options' => array(
				0=>t('No'), 
				1=>t('Yes'),
				),
		);
		$element[$element_id]['order'] = array(
			'#type' => 'textfield',
			'#title' => 'Sort Order',
			'#size' => 5,
			'#maxlength' => 3,
		);		
		
		$element[$element_id]['values'] = array(
			'#type' => 'fieldset',
			'#title' => "Option Values",
			'#prefix' => "<div id='custom-options-option-value-" . $element_id . "'>",
			'#attributes' => array(
				'class' => array(
					'es-custom-option-type',
				),
			),
			'#suffix' => "</div>",
		);
    /*$element[$element_id]['values']['option_select'] = array(
      '#type' => 'submit',
      '#value' => t('Select Option Values'),
      '#submit' => array('renderSelectTemplate'),
      '#name' => 'option_select_' . $element_id, 
      '#ajax' => array(
        'callback' => 'renderSelectTemplateCallback',
        'wrapper' => 'names-fieldset-wrapper',
      ),
      '#prefix' => '<!--<attributes><id>'. $element_id . '</id></attributes>-->',
    );*/
				//if(!empty($form_state['values']['custom_options']['option'][$element_id]['type'])) {
		 
		 if(isset($form_state['values']['custom_options']['option'][$element_id][$element_id]['type'])) {
			
			$option_value_type = $form_state['values']['custom_options']['option'][$element_id][$element_id]['type'];
			 watchdog('product id options renderfx','type: %value_type id: %id <pre>%options</pre>',array('%options'=>print_r($form_state,true),'%id'=>$element_id, '%value_type'=>$option_value_type),WATCHDOG_NOTICE);
			if($option_value_type == "field") {
	    $element[$element_id]['values']['values_type'] = array(
	    	'#type' => 'textfield',
	      '#title' => t('Who was the first president of the U.S.?' . $option_value_type ),
	      );
			} elseif($option_value_type=="drop_down") {
			  $element[$element_id]['values']['values_type'] = array(
	    	'#type' => 'textfield',
	      '#title' => t('Who dropdown'),
	      );
			} else {
					    $element[$element_id]['values_type'] = array(
	    	'#type' => 'textfield',
	      '#title' => t('Who was telse' . $option_value_type ),
	      );
			}
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
			array('label' => 'Drop-down','value' => 'drop_down'),
			array('label' => 'Radio Buttons','value' => 'radio'),
			array('label' => 'Checkbox','value' => 'checkbox'),
			array('label' => 'Multiple Select','value' => 'multiple'),
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
