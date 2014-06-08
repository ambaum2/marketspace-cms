<?php

/**
 * @file
 * Webform module fieldset component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_ms_fieldset() {
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
            'help_video_embed' => '',
        ),
    );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_ms_fieldset($component) {
    $form = array();
    $form['extra']['help_video_embed'] = array(
        '#type' => 'textarea',
        '#title' => t('Help Video Embed'),
        '#default_value' => isset($component['extra']['help_video_embed']) ? $component['extra']['help_video_embed'] : '1',
    );
    $form['display']['collapsible'] = array(
        '#type' => 'checkbox',
        '#title' => t('Collapsible'),
        '#default_value' => $component['extra']['collapsible'],
        '#description' => t('If this fieldset is collapsible, the user may open or close the fieldset.'),
        '#weight' => 0,
        '#parents' => array('extra', 'collapsible'),
    );
    $form['display']['collapsed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Collapsed by Default'),
        '#default_value' => $component['extra']['collapsed'],
        '#description' => t('Collapsible fieldsets are "open" by default. Select this option to default the fieldset to "closed."'),
        '#weight' => 3,
        '#parents' => array('extra', 'collapsed'),
    );
    return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_ms_fieldset($component, $value = NULL, $filter = FALSE) {
    $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
    $help_text = (!empty($component['extra']['help_video_embed'])) ?
        '&nbsp;<button type="button" class="btn btn-warning btn-lg pull-right" data-toggle="modal" data-target=".ms-detail-modal-' . $component['cid'] . '">
        <span class="glyphicon glyphicon-thumbs-up"></span>&nbsp; Help Me</button>
        <div class="ms-modal modal fade ms-detail-modal-'. $component['cid'] . '" role="dialog" aria-labelledby="myLargeModalLabel" aria-hidden="true">
            <div class="modal-dialog modal-lg">
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                        <h4 class="modal-title" id="myModalLabel">'. $component['name'] .' Help</h4>
                    </div>
                    <div class="modal-body">
                        '. $component['extra']['help_video_embed'] . '
                    </div>
                </div>
            </div>
        </div>' : '';
    $element = array(
        '#type' => 'fieldset',
        '#title' => $filter ? _webform_filter_xss($component['name']) . $help_text : $component['name'] . $help_text,
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : NULL,
        '#weight' => $component['weight'],
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
        '#collapsible' => $component['extra']['collapsible'],
        '#collapsed' => $component['extra']['collapsed'],
        '#attributes' => array('class' => array('webform-component-fieldset'), 'id' => 'webform-component-' . $component['form_key']),
        '#pre_render' => array('webform_ms_fieldset_prerender', 'webform_element_title_display'),
        '#translatable' => array('title', 'description'),
    );

    return $element;
}

/**
 * Pre-render function to set a ms_fieldset ID.
 */
function webform_ms_fieldset_prerender($element) {
  $element['#attributes']['id'] = 'webform-component-' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_ms_fieldset($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#theme_wrappers' => array('webform_element_text'),
      '#translatable' => array('title'),
    );
  }
  else {
    $element = _webform_render_ms_fieldset($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}
