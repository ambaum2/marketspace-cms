<?php
class product_custom_options_templates extends product_custom_options {
		
	/*
	 * 
	 * $custom_option - (Object) used
	 * for prepopulating values
	 * $options list of all types - may come from 
	 * $parent->getCustomOptionType($options,$filter_values);
	 * $option_id is this existing option? TODO is this necessary?
	 */
	public function custom_option_type_template($element_id, $option_id = false, $custom_option = null, $option_types = null) {
		if($option_types == null) {
			$options = parent::getAllCustomOptions();
			$option_types = parent::getCustomOptionType($options);
		}
		$element = array(
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
			'#options' => $option_types,
			'#disabled' => ($option_id) ? TRUE : FALSE,
      '#ajax' => array(
        'callback' => 'getCustomOptionsTypeTemplate',//'getOptionsTypeTemplate',
        'wrapper' => 'custom-options-option-value-' . $element_id,
      ),
      '#prefix' => '<!--<attributes><id>'. $element_id . '</id></attributes>-->',
		);
    
		$element['required'] = array(
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
		);
		
		return $element;
	}
}
	