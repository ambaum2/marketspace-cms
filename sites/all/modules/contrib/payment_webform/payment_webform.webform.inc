<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_payment_webform() {
  return array(
    'extra' => array(
      'payment_currency_code' => 'XXX',
      'payment_description' => '',
      'payment_line_items' => array(),
      'private' => TRUE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_payment_webform($component) {
  $form['extra']['payment_currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => array(2=>"Southern Illinois", 45=>'East Central Illinois'),
    '#default_value' => $component['extra']['payment_currency_code'],
    '#required' => TRUE,
  );
  $form['extra']['payment_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $component['extra']['payment_description'],
    '#required' => TRUE,
  );
  $form['extra']['payment_line_items'] = array(
    '#type' => 'payment_line_item',
    '#title' => t('Line items'),
    '#cardinality' => 0,
    '#default_value' => $component['extra']['payment_line_items'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements _webform_render_[component]().
 */
function _webform_render_payment_webform($component, $value = NULL, $filter = TRUE) {
  global $user;

  $form = array(
    '#type' => 'textfield',
    '#title' => $component['name'],
    '#default_value' => is_array($value) ? reset($value) : 0,
    '#required' => $component['mandatory'],
    '#payment_line_items' => $component['extra']['payment_line_items'],
    '#payment_currency_code' => $component['extra']['payment_currency_code'],
    '#payment_add_page_path' => 'payment_webform/pay/' . $component['nid'] . '/' . $component['cid'],
    '#payment_load_callback' => 'payment_webform_load',
    '#payment_load_arguments' => array($component['cid'], $user->uid),
  );

  return $form;
}

/**
 * Implements _webform_submit_[component]().
 */
function _webform_submit_payment_webform($component, $value) {
  payment_webform_delete_by_pid($value);

  return $value;
}