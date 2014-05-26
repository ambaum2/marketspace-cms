<?php

/**
 * @file
 * template.php
 */

/**
 * @param $vars
 * @param $hooks
 */
function cm_preprocess_page(&$vars){
    if (in_array(arg(0), array('manage-products', 'products-reports', 'products-reports-grid'))) {
        // name will be "manage-products.tpl.php".
       $vars['theme_hook_suggestions'][] = str_replace(array('-'), '_', arg(0));
    }
    //$vars['ms_product_reports_nav_2'] = menu_tree(variable_get('ms_product_reports_links_source', 'menu-product-reports'));
    $vars['ms_product_reports_nav'] = menu_navigation_links('menu-product-reports');

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
