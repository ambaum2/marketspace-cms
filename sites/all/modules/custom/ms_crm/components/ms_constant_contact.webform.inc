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
        '#required' => true,
        '#options' => array(
            '' => t("None"),
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
            'notes' => t('Note'),
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
    } elseif(isset($_POST['_triggering_element_name'])) {
        $field = new MS_Constant_Contact_Contacts_Fields($_POST['extra']['field_type'], $component);
    }
    if(isset($field)) {
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
        /*$component = MS_Constant_Contact_Contacts_Fields::BuildComponent($form_state);
        $field = new MS_Constant_Contact_Contacts_Fields($element_key, $component, $form_state);
        $f = $field->GetSettingsForm();
        $form['extra']['cc']['settings'] = $f;
        $unprocessed_form = $form;
        empty($form_state['manadatory']) ?  $form_state['manadatory'] = 0 : '';
        //$form = form_builder($form_state['input']['form_id'], $form, $form_state);
        $element = form_builder($form_state['input']['form_id'], $form['extra']['cc'], $form_state);
        form_set_cache($form['#build_id'], $unprocessed_form, $form_state); //you MUST set the cache or the form form_state['values'] never gets set right see the end of drupal_process_form in form.inc
        $html = drupal_render($element);
        $commands[] = ajax_command_replace('#cc_field_settings_form', $html);
        return array('#type' => 'ajax', '#commands' => $commands);*/
        return $form['extra']['cc'];
    } catch(Exception $e) {
        watchdog('cc_settings', "<pre>%form_state</pre>", array('%form_state' => print_r($form, true)), WATCHDOG_INFO);
    }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_ms_crm_cc($component, $value = NULL, $filter = FALSE) {
    $cc_field = new MS_Constant_Contact_Contacts_Fields($component['extra']['field_type'], $component);
    $element = $cc_field->fieldType->GetDisplayFields();
    //drupal_write_record()
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

/**
 * A hook for changing the input values before saving to the database.
 *
 * Webform expects a component to consist of a single field, or a single array
 * of fields. If you have a component that requires a deeper form tree
 * you must flatten the data into a single array using this callback
 * or by setting #parents on each field to avoid data loss and/or unexpected
 * behavior.
 *
 * Note that Webform will save the result of this function directly into the
 * database.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   The POST data associated with the user input.
 * @return string An array of values to be saved into the database. Note that this should be
 */
function _webform_submit_ms_crm_cc($component, $value) {
    $ms_cc = new MS_Constant_Contact_Contacts_Submission();
    $result = $ms_cc->GetSingleSubmissionValue($value);
    return $result;
}