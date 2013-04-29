<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_es_custom_option() {
  return array(
    'extra' => array(
      'private' => FALSE,
    ),
  );
}
function es_custom_option_webform_component_presave(&$component) {
  /*echo "POSTPOSTPOST<pre>" . print_r($_POST,true) . "</pre>";	

  echo "<pre>" . print_r($component,true) . "</pre>";
  die;*/
}
/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_es_custom_option($component, $form_state) {
	$product_custom_options = new product_custom_options();
  $form['description'] = array(
    '#markup' => '<div>' . t('This example shows an add-more and a remove-last button. The <a href="!ajax">AJAX version</a> does it without page reloads; the <a href="!multistep">non-js version</a> is the same code but simulates a non-javascript environment, showing it with page reloads.',
      array('!ajax' => url('examples/ajax_example/add_more'), '!multistep' => url('examples/ajax_example/add_more_no_js')))
    . '</div>',
  );

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;
  $form['custom_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('People coming to the picnic'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="names-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_names'])) {
    $form_state['num_names'] = 0;
  }
	$options = $product_custom_options->getAllCustomOptions();
	
  for ($i = 0; $i < $form_state['num_names']; $i++) {
  	$form['custom_options']['option'][$i] = $product_custom_options->renderOptionInfoForm($options, $i, $form_state);
		/*if(!empty($form_state['values']['custom_options']['option'][$i]['type'])) {
		  watchdog('product options in if for for','<pre>%options</pre>',array('%options'=>print_r($form_state,true),WATCHDOG_NOTICE));			
			$option_value_type = $form_state['values']['custom_options']['option'][$i]['type'];
	    //$form['custom_options']['option'][$i]['values']['president'] = array(
	    // '#markup' => t('Who was the first president of the U.S.?' . $option_value_type ),
      //);
    
		}*/
    /*$form['custom_options']['option'][$i] = array(
      '#title' => t('Custom Option'),
      '#type' => 'fieldset',
      '#prefix' => '<!--<attributes><id>'. $i . '</id></attributes>-->',
      
    );
    $form['custom_options']['option'][$i]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('title'),
    );
    $form['custom_options']['option'][$i]['Type'] = array(
      '#type' => 'textfield',
      '#title' => t('type'),
    );*/
  }
  $form['names_fieldset']['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('ajax_example_add_more_add_one'),
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => array(
    	'method' => 'append',
      'callback' => 'ajax_example_add_more_callback',
      'wrapper' => 'names-fieldset-wrapper',
    ),
    '#prefix' => '<!--<attributes><id>'. $i . '</id></attributes>-->',
  );
  if ($form_state['num_names'] > 1) {
    $form['names_fieldset']['remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('ajax_example_add_more_remove_one'),
      '#ajax' => array(
        'callback' => 'ajax_example_add_more_callback',
        'wrapper' => 'names-fieldset-wrapper',
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/*
 * callback for type select
 * inserts the appropriate custom option
 * template when changed
 */
 function getOptionsTypeTemplate($form, $form_state) {
	$xml = str_replace(array('<!--','-->'),'',$form_state['triggering_element']['#prefix']);
	$attributes = simplexml_load_string($xml);
	$attribute_id = is_object($attributes->id) ? "yes" : "not obj";//$attributes->id;
	//key_exists('id', $attributes) ? (int)$attributes->id : 0;
	
	watchdog('custom option ajax callback', 'attr id %attr_id : values for xmlstring %xml_string attributes arr <pre>%attributes</pre> xml <pre>%xml</pre> ', array('%xml'=>print_r($form_state,true), '%xml_string'=>$xml, '%attributes' => print_r($attributes,true)
		,'%attr_id'=>((int)$attributes->id)),WATCHDOG_NOTICE);
  return $form['custom_options']['option'][((int)$attributes->id)][((int)$attributes->id)]['values']; 	
 }
/**
 * Callback for both ajax-enabled buttons.
 *
 * get the callback trigger attributes xml. Make an object. Then call on that to be rebuilt
 * with the return value
 * WARNING this will fire when other unrelated ajax
 * elements are triggered. Therefore add the check for
 * _triggering_element_value == "Add one more" so we
 * we only increment num_names by 1 if the add more
 * button is clicked
 * 
 * @TODO - support more than 10 custom options (see element id below)
 */
function ajax_example_add_more_callback($form, $form_state) {

	//$form_state['custom_option_values'][substr($_POST['_triggering_element_name'],14,1)] = $_POST['custom_options']['option'][substr($_POST['_triggering_element_name'],14,1)]['type'];
	if(isset($form_state['input']['_triggering_element_value'])) {
		if($form_state['input']['_triggering_element_value'] == "Add one more") {	
			$xml = str_replace(array('<!--','-->'),'',$form_state['triggering_element']['#prefix']);
			$attributes = simplexml_load_string($xml);
			(int)$attribute_id = key_exists('id', $attributes) ? (int)$attributes->id : 0;
		  return $form['custom_options']['option'][($attribute_id - 1)];
		}
	} elseif(!(strpos($form_state['input']['_triggering_element_name'],'[type]') === false)) { //if this is a custom option type callback
			// = $_POST['custom_options']['option'][substr($_POST['_triggering_element_name'],14,1)]['type'];
			$element_id = substr($form_state['input']['_triggering_element_name'],strpos($form_state['input']['_triggering_element_name'],'[option][')+9,1);
				  watchdog('custom option add more callback HIT!', 'ele id form trigger name: %elementid || %numcount attr id %attr_id : xml <pre>%xml</pre> ', array('%numcount'=>$form_state['num_names'],
	  	'%xml' => print_r($form,true),'%elementid' => $element_id),WATCHDOG_NOTICE);
		return $form['custom_options']['option'][0][0]['values'];
	}
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 * 
 * WARNING this will fire when other unrelated ajax
 * elements are triggered. Therefore add the check for
 * _triggering_element_value == "Add one more" so we
 * we only increment num_names by 1 if the add more
 * button is clicked
 */
function ajax_example_add_more_add_one($form, &$form_state) {
		  watchdog('custom option add more HIT!', '%numcount attr id %attr_id : xml <pre>%xml</pre> ', array('%numcount'=>$form_state['num_names'],
	  	'%xml' => print_r($form_state,true)),WATCHDOG_NOTICE);
	if(isset($form_state['input']['_triggering_element_value'])) {
		if($form_state['input']['_triggering_element_value'] == "Add one more") {
		  isset($form_state['num_names']) ? $form_state['num_names']++ : $form_state['num_names'] = 1; //TODO:-this is undefined initially so may want conditional
		  $form_state['rebuild'] = TRUE;
		}
	}
	
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function ajax_example_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_names'] > 1) {
    $form_state['num_names']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function ajax_example_add_more_submit($form, &$form_state) {
  $output = t('These people are coming to the picnic: @names',
    array('@names' => implode(', ', $form_state['values']['names_fieldset']['name'])) );
  drupal_set_message($output);
}
/**
 * Implements _webform_render_[component]().
 */
function _webform_render_es_custom_option($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;

}

function _webform_display_es_custom_option($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title', 'field_prefix', 'field_suffix'),
  );
}


