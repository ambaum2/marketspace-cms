<?php
class product_custom_options extends product {
	public static $ajax_option;
	/**
   * @param options | array 
	 * options is an array of objects which have value label like:
	 *     [6] => stdClass Object
        (
            [label] => Multiple Select
            [value] => multiple
        )
	 * select type for future can specify whether select, checkbox, radio etc
	 * loop through custom options and values
	 * and add, update, or delete array 
	 */
	
	public function updateCustomOptions($options) {
		$magento_custom_options = new magento_custom_options;
		//echo "<pre>" . print_r($options,true) . '</pre>';
		foreach($options['custom_options'] as $key=>$value) {
			if(!empty($value['type']) && is_numeric($key)) { //must check is numeric or else will sibling form elements in this
				$data = $value;
				if($value['delete'] == 1 && $value['magento_option_id'] > 0) { //delete option if this is checked no questions
					$magento_custom_options->delete_product_custom_option($value['magento_option_id']);
				} elseif(!in_array($value['type'], array('drop_down', 'radio', 'checkbox', 'multiple'))) {
						$data['additional_fields'] = array($value['values']);
						//echo "<pre>" . print_r($data,true) . '</pre>';
					if($value['magento_option_id'] > 0) {
						$result = $magento_custom_options->update_custom_option($value['magento_option_id'], $data);
					} else { //new simple option
					  //print "fired add";
						//echo "<pre>" . print_r($data,true) . '</pre>';
						$result = $magento_custom_options->add_custom_option($options['product_id'], $data);
					}
				} else { //loop through option values and add, edit, delete
					//need to remove sibling element "add option" or else magento api fails b/c of invalid key		
					foreach($value['values'] as $okey => $ovalue) {
						if(is_numeric($okey)) {
							$data['additional_fields'][$okey] = $ovalue;
							//echo "<pre>data item" . $okey . print_r($data['additional_fields'][$okey],true) . '</pre>';
							if($data['additional_fields'][$okey]['delete'] == 1) {
								//echo "<pre>select type thing" . print_r($data,true) . '</pre>';
								$result = $magento_custom_options->remove_custom_option_value($data['additional_fields'][$okey]['value_id']);
								unset($data['additional_fields'][$okey]); //remove from array so not updated 
							}
						}
					}
					//drupal_set_message( "<pre>select pre update/add" . print_r($data,true) . '</pre>', 'status');
					if($value['magento_option_id'] > 0) {
						$result = $magento_custom_options->update_custom_option($value['magento_option_id'], $data);
					} else {
						$result = $magento_custom_options->add_custom_option($options['product_id'], $data);
					}
				}
			}
		}
	}
  /**
   * create custom options select form
   * 
   * @param options | array
   *  array of options keted [number] => [value]
   * @param element_id | integer
   *  id of option
   * @param title | string
   *  name of option from magento
   * @param description | string
   *  optinal description for element
   * @param filter_values | array
   *  optional filter out certain values
   * @param select_type | string
   *  can choose the type of select select, radio, multi etc
   * @param defaults | array
   *  array of default option or options
   * @return array 
   *  the renderable array for drupal form
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
  
  /**
   * create the options array for the select elements form element
   * 
   * @param options | array
   *  array of keyed option 
   * @param filter_values | array
   *  optional filter out certain values
   * @return array
   *  array of keyed values
   * 
   */
	public function getCustomOptionType($options, $filter_values = array()) {
		$all_options = array('none' => "Select a Type");	
		foreach($options as $key=>$value) {
			if(!(in_array($value->value, $filter_values))) {
				$all_options[$value->value] = $value->label;
			}
		}
		return $all_options;
	}
	/**
	 * create a template with option name, type, 
	 * required and order
   * @param options | array
   *  array of options keted [number] => [value]
   * @param element_id | integer
   *  id of option
   * @param form_state | array
   *  current form state so we can get custom options and other 
   *  form variables
   * @param custom_option | object
   *  custom option object for prefilling the option form
   * @param filter_values | array
   *  optional filter out certain values
   * @return array 
   *  the renderable array for drupal form
   * @todo this seems insane
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

    if(isset($custom_option->type)) {
      $element['type']['#default_value'] = $custom_option->type;
    } elseif(isset($form_state['values']['custom_options'][$element_id]['type'])) {
      $element['type']['#default_value'] = $form_state['values']['custom_options'][$element_id]['type'];
			//$element['type']['#default_value'] = $form_state['input']['custom_options'][$element_id]['type'];
    }
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
			} elseif($option_value_type=="drop_down" || $option_value_type=="checkbox" 
		       || $option_value_type=="radio" || $option_value_type=="multiple" ) {

          //option values from magento
          $custom_options_count = 0;
          if(isset($custom_option->additional_fields) ) { //&& !(isset($form_state['input']['custom_options'][$element_id]['values']))
            if(count($custom_option->additional_fields)) {
              foreach($custom_option->additional_fields as $key=>$val) {
                $element['values'][$key] = $product_custom_options_templates->custom_option_value_select_type_template($key, $element_id, $custom_option->additional_fields[$key]->value_id, $custom_option);  
                $custom_options_count++;
								//watchdog('mage create', '%num_count count and <pre>%form</pre><br />',array('%form' => print_r($form_state,true), '%num_count'=>($custom_options_count)),WATCHDOG_NOTICE); 
              }
            }
          }
          //newly added in drupal pending save for magento
          //check for new added values
          if(isset($form_state['input']['custom_options'][$element_id]['values'])) { //add pending add options to magento options
            //$custom_options_count = count($form_state['input']['custom_options'][$element_id]['values']);
          }
          if(isset($form_state['input']['custom_options'][$element_id]['values'])) {
            foreach($form_state['input']['custom_options'][$element_id]['values'] as $key=>$option) {
            	if(isset($form_state['input']['custom_options'][$element_id]['values'][$key]['value_id'])) {
            		$value_id = $form_state['input']['custom_options'][$element_id]['values'][$key]['value_id'];
								//$custom_options_count--;
							} else {
								$value_id = null;
							}
            	$element['values'][$key] = $product_custom_options_templates->custom_option_value_select_type_template($key, $element_id, $value_id);
							
              //$custom_options_count++;
							//watchdog('form state create', '%num_count count and <pre>%form</pre><br />',array('%form' => print_r($form_state,true), '%num_count'=>($custom_options_count)),WATCHDOG_NOTICE); 
            }
          } 
          
          //check the form state and see if option_value_callback has called to add a value
          if(isset($form_state['triggering_element']['#value'])) {
    	      $xml = str_replace(array('<!--','-->'),'',$form_state['triggering_element']['#prefix']);
			      $attributes = simplexml_load_string($xml);  
            if($form_state['triggering_element']['#value'] == 'Add Value' && $element_id == $attributes->parent_id) {
            	if(isset($form_state['input']['custom_options'][$element_id]['values'])) {
            		$new_option_position = count($form_state['input']['custom_options'][$element_id]['values']); //0 ?
							} else {
								$new_option_position = 0;
							}
              $element['values'][$new_option_position] = $product_custom_options_templates->custom_option_value_select_type_template($new_option_position, $element_id);
              
              /*watchdog('value option add', '%num_count count and <pre>%form</pre><br />'
              ,array('%form' => print_r($form_state,true), '%num_count'=>($new_option_position . ' ele id ' . $element_id))
              ,WATCHDOG_NOTICE);  */            
            } elseif($form_state['triggering_element']['#value'] == 'Delete') {    
								unset($element['values'][(int)$attributes->id]);
								array_values($element['values'][(int)$attributes->parent_id]);
					      //unset($form_state['input']['custom_options'][(int)$attributes->parent_id]['values'][(int)$attributes->id]);
								/*watchdog('value option unset', '%num_count count and <pre>%form</pre><br />'
									,array('%form' => print_r($form_state,true), '%num_count'=>($custom_options_count))
									,WATCHDOG_NOTICE); */ 
            }
          } else {
          	/*watchdog('value option failed add/delete', '%num_count count and <pre>%form</pre><br />'
              ,array('%form' => print_r($form_state,true), '%num_count'=>(2222 . ' ele id ' . $element_id))
              ,WATCHDOG_NOTICE);   */  
          }
          
          
          $element['values']['add'] = array(
            '#type' => 'button',
            '#value' => t('Add Value'),
            '#ajax' => array(
              'callback' => 'option_value_callback',//'getOptionsTypeTemplate',
              'wrapper' => 'edit-custom-options-'. $element_id . '-values-add',//'custom-options-option-' . ($key+1) . '-value-type-' . ($key+1),
              'method' => 'before',
            ),
			      '#name' => 'add_option_value_' . $element_id,
			      '#prefix' => '<!--<attributes><parent_id>' . $element_id . '</parent_id></attributes>-->',
			      //shouldn't need <id>'. $key . '</id> anymore but will keep here for reference
			      //it will be null for newly added options
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

	/**
	 * create the custom option value templates
	 * one for text, select, and date types
	 * they are rendered all at once but hidden
   * @return object
   *  returns all custom options as an object when they are 
   *  needed as an object instead of an array like the template
	 * @TODO - file type
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
