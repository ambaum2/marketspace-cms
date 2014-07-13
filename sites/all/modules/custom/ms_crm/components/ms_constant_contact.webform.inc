<?php

/**
 * @file
 * Webform module fieldset component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_ms_crm_cc() {
    return array(
        'name' => '',
        'form_key' => NULL,
        'pid' => 0,
        'weight' => 0,
        'extra' => array(
            'title_display' => 0,
            'collapsible' => 0,
            'collapsed' => 0,
            'description' => '',
            'private' => FALSE,
        ),
    );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_ms_crm_cc($component) {
    $form = array();
    $form['#id'] = 'ms_cc_form';
    $form['extra']['field_type'] = array(
        '#type' => 'select',
        '#title' => t('Type of Field'),
        '#options' => array(
            'addresses' => t("Addresses"),
            'lists' => t('Lists'),
            'cell_phone' => t('Cell Phone'),
            'company_name' => t('Company Name'),
            'email_addresses' => t('Email Addresses'),
            'fax' => t('Fax'),
            'first_name' => t('First Name'),
            'home_phone' => t('Home Phone'),
            'job_title' => t('Job Title'),
            'last_name' => t('Last Name'),
            'middle_name' => t('Middle Name'),
            'prefix_name' => t('Prefix'),
            'work_phone' => t('Work Phone Number'),
            'custom_fields' => t('Custom Field')
        ),
        '#default_value' => isset($component['extra']['field_type']) ? $component['extra']['field_type'] : '1',
        '#ajax' => array(
            'callback' => 'ms_crm_cc_settings',
            'method' => 'replace',
            'wrapper' => 'cc_field_settings_form',
        ),
    );

    $form['extra']['cc'] = array(
        '#type' => 'fieldset',
        '#title' => t('Field Settings'),
        '#tree' => true,
        '#attributes' => array('id' => array('cc_field_settings_form')),
    );

    if(isset($component['extra']['field_type']) && isset($component['cid'])
        && !isset($_POST['_triggering_element_name'])) { //do not build the settings form if this is an ajax request
        $field = new MS_Constant_Contact_Contacts_Fields($component['extra']['field_type'], $component);
        $f = $field->GetSettingsForm();
        $form['extra']['cc']['settings'] = $f;
    }
    return $form;
}

/**
 * ajax callback to process form
 * get around ajax problems by replacing the whole form and
 * use form_builder so name attributes get processed correctly
 * @param $form
 * @param $form_state
 * @return array|string
 */
function ms_crm_cc_settings(&$form, &$form_state) {
    $commands = array();
    $element_key = $form_state['triggering_element']['#value'];
    try {
        $component = MS_Constant_Contact_Contacts_Fields::BuildComponent($form_state);
        $field = new MS_Constant_Contact_Contacts_Fields($element_key, $component, $form_state);
        $f = $field->GetSettingsForm();
        $form['extra']['cc']['settings'] = $f;
        $element = form_builder($form_state['input']['form_id'], $form, $form_state);
        $html = drupal_render($element);
        $commands[] = ajax_command_replace('#ms_cc_form', $html);
        return array('#type' => 'ajax', '#commands' => $commands);
    } catch(Exception $e) {
        watchdog('cc_settings', "<pre>%form_state</pre>", array('%form_state' => print_r($form, true)), WATCHDOG_INFO);
    }
}
/**
 * Implements _webform_render_component().
 */
function _webform_render_ms_crm_cc($component, $value = NULL, $filter = FALSE) {
//    $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
//    $cid = $component['cid'];
    $cc_field = new MS_Constant_Contact_Contacts_Fields($component['extra']['field_type'], $component);
    $element = $cc_field->fieldType->GetDisplayFields();
    return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_ms_crm_cc($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#theme_wrappers' => array('webform_element_text'),
      '#translatable' => array('title'),
    );
  }
  else {
    $element = _webform_render_ms_crm_cc($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}
