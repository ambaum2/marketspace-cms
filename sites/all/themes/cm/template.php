<?php

/**
 * @file
 * template.php
 */

/**
 * @param $vars
 * @param $hooks
 */
function cm_process_page(&$vars, $hooks){
    if (in_array(arg(1), array('manage-products'))) {
        // name will be "manage-products.tpl.php".
        $vars['theme_hook_suggestions'][] = '' . arg(1);
    }
    $excluded_menu_items = array('File browser');
    for($i=0; $i < count($vars['tabs']['#primary']); $i++) {
        if(isset($vars['tabs']['#primary'][$i])) {
            if($vars['tabs']['#primary'][$i]['#link']['title'] == 'File browser') {
                unset($vars['tabs']['#primary'][$i]);
            }
        }
    }
}

/**
 * @param $form
 * @param $form_state
 * @param $form_id
 */
function cm_form_alter(&$form, $form_state, $form_id) {
    if($form_id == 'user_profile_form') {
        unset($form['picture']);
    }
}
