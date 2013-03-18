<?php
class product_custom_options extends product {
	
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
	public function renderOptionInfoForm($options, $element_id, $filter_values = array()) {
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
		return $element;
	}
}
