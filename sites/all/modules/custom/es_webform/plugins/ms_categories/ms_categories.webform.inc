<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_ms_categories() {
    return array(
        'extra' => array(
            'private' => FALSE,
        ),
    );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_ms_categories($component) {

    $webform = node_load($component['nid']);
    if(isset($webform->field_market_space_settings['und'][0]['attribute_set_id'])) {
        $form['extra']['magento_attribute_code_name'] = array(
            '#type' => 'hidden',
            '#value' => "categories",
        );
        $form['extra']['magento_categories'] = array(
            '#type' => 'fieldset',
            '#title' => 'Categories',
            '#tree' => true,
        );
        $Api = new MS_Api();
        $categories = $Api->apiTokenCall("/ms-api/categories/tree/lists/Nested/2/10", false);
        dpm($categories);
        $category_colors = array(
            1 => '#FBF6D9',
            2 => '#FBF6D9',
            3 => '#98AFC7',
            4 => '#C7B097',
            5 => '#7A6C87',
        );
        foreach($categories as $key => $category) {
            //$categories_checkboxes[$category->category_id] = $category->name;

            $form['extra']['magento_categories'][$category->category_id] = array(
                '#type' => 'checkbox',
                '#title' => t($category->name),
                '#prefix' => '<div style="margin-left:'. $category->level * 10 . 'px;background:'.$category_colors[$category->level].'" class="btn-group" data-toggle="buttons"><label class="btn btn-primary">',
                '#suffix' => '</label></div>',
                '#default_value' => $webform->webform['components'][$component['cid']]['extra']['magento_categories'][$category->category_id],
            );
        }
    } else {
        $form['extra']['warning'] = array(
            '#type' => 'item',
            '#title' => t('Magento Attribute'),
            '#markup' => "<p style='color:red;font-weight:900;'>You must set an attribute set id before you can choose categories</p>",
        );
    }
    return $form;
}

/**
 * Implements _webform_render_[component]().j
 * $component has information about component
 * $form state is a hack of webform to get it to show up
 */
function _webform_render_ms_categories($component, $value = NULL, $filter = TRUE, $form_state) {
    $webform = node_load($component['nid']);
    $category_list = array();
    foreach($webform->webform['components'][$component['cid']]['extra']['magento_categories'] as $key => $value) {
        if($value == 1)
            $category_list[] = $key;
    }
    $category_list_string = implode(',', $category_list);
    $Api = new MS_Api();
    $categories = $Api->apiTokenCall("/ms-api/categories/tree/lists/filtered/Nested/1/10/$category_list_string", false);
    $form['categories'] = array(
        '#type' => 'fieldset',
        //'#attributes' => array('class' => array('btn-group'), 'data-toggle' => 'buttons'),
        // => array('ms_categories_remove_markup'),
    );
    foreach($categories as $key => $category) {
        //$categories_checkboxes[$category->category_id] = $category->name;
        $form['categories'][$category->category_id] = array(
            '#type' => 'checkbox',
            '#title' => t($category->name),
            '#prefix' => '<div style="margin-left:'. $category->level * 10 . 'px;">',
            '#suffix' => '</div>',
            '#default_value' => isset($form_state['values']['submitted'][$component['cid']]['ms_categories'][$category->category_id])
                ? $form_state['values']['submitted'][$component['cid']]['ms_categories'][$category->category_id] : '',
        );
    }

    isset($_POST['form_build_id']) ? $form_build_id = $_POST['form_build_id'] : $form_build_id = "";
    return $form;
}

/**
 * remove the panel body div that is wrapping the categories
 * @param $markup
 * @param $element
 * @return mixed|string
 */
function ms_categories_remove_markup($markup, $element) {
    $markup = str_replace('<div class="panel-body">', '', $markup);
    $markup = substr_replace($markup, '', strrpos($markup, "</div>"), 6);
    return $markup;
}

function _webform_display_ms_categories($component, $value, $format = 'html') {
    return array(
        '#title' => $component['name'],
        '#weight' => $component['weight'],
        '#format' => $format,
        '#value' => isset($value[0]) ? $value[0] : '',
        '#translatable' => array('title', 'field_prefix', 'field_suffix'),
    );
}