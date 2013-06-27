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
    
		$element['is_require'] = array(
			'#type' => 'select',
			'#title' => 'Is Required',
			'#options' => array(
				0=>t('No'), 
				1=>t('Yes'),
			),
			'#default_value' => isset($custom_option->is_require) ? $custom_option->is_require : '',
		);
		$element['sort_order'] = array(
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
					'es-custom-option-value-container',
				),
			),
			'#tree' => TRUE,
			'#suffix' => "</div>",
		);	
		return $element;
	}

/*
 * custom options select, multi, radio
 * and checkbox template
 */
public function custom_option_value_select_type_template($key, $element_id, $value_id = null, $custom_option = null) {
  $element = array(
    '#type' => 'fieldset',
    '#title' => "Value",
    '#attributes' => array(
      'class' => array(
        'es-custom-option-type',
      ),
    ),
    '#prefix' => "<div id='custom-options-option-" . $element_id . "-value-type-" . $key . "'>",
    '#suffix' => "</div>",         
  );
  $element['title'] = array(
  '#type' => 'textfield',
  '#title' => t('Title'),
  '#default_value' => isset($custom_option->additional_fields[$key]->title) ? $custom_option->additional_fields[$key]->title : '',
  '#size' => 5,
  );
  $element['price'] = array(
    '#type' => 'textfield',
    '#title' => t('Price'),
    '#default_value' => isset($custom_option->additional_fields[$key]->price) ? $custom_option->additional_fields[$key]->price : '',
    '#size' => 5,
    );
  $element['price_type'] = array(
    '#type' => 'select',
    '#title' => 'Price Type',
    '#options' => array(
      'fixed'=>t('Price'),
      'percent'=>t('Percent'),
    ),
    '#default_value' => isset($custom_option->additional_fields[$key]->price_type) ? $custom_option->additional_fields[$key]->price_type : '',
  );  
  $element['sku'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU'),
    '#default_value' => isset($custom_option->additional_fields[$key]->sku) ? $custom_option->additional_fields[$key]->sku : '',
    '#size' => 5,
  );
  $element['sort_order'] = array(
    '#type' => 'textfield',
    '#title' => t('Sort Order'),
    '#default_value' => isset($custom_option->additional_fields[$key]->sort_order) ? $custom_option->additional_fields[$key]->sort_order : '',
    '#size' => 5,
  );
	$element['value_id'] = array(
		'#type'=>'hidden', 
		'#value' => ($value_id) ? $value_id : '',
	);
  if($value_id) {
    $element['delete'] = array(
      '#type' => 'checkbox',
      '#title' => 'Delete',
      '#description' => t('Existing option values must be checked if you want to delete them. Click Save Product below to confirm'),
    );                
  } else {
    $element['delete'] = array(
      '#type' => 'button',
      '#value' => t('Delete'),
      '#ajax' => array(
        'callback' => 'option_value_callback',//'getOptionsTypeTemplate',
        'wrapper' => 'custom-options-option-value-' . $element_id,//'custom-options-option-' . $element_id . 'value-type-' . $key, //
      ),
      '#name' => 'delete_option_value_' . $key,
      '#prefix' => '<!--<attributes><id>'. $key . '</id><parent_id>' . $element_id . '</parent_id></attributes>-->',
    );
  }
  return $element;
}

}
	