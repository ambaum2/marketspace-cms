<?php

/**
 * @file
 * template.php
 */

function cm_process_page(&$vars, $hooks){
  //krumo($vars);
  $excluded_menu_items = array('File browser');
  for($i=0; $i < count($vars['tabs']['#primary']); $i++) {
    if(isset($vars['tabs']['#primary'][$i])) {
      if($vars['tabs']['#primary'][$i]['#link']['title'] == 'File browser') {
        unset($vars['tabs']['#primary'][$i]);
      }
    }
  }
}

function cm_form_alter(&$form, $form_state, $form_id) {
  if($form_id == 'user_profile_form') {
    //echo "<pre>" . print_r($form, true) . "</pre>";
    unset($form['picture']);
  }
}
