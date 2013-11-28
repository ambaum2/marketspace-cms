<?php

/**
 * Implements hook_views_data().
 */
function module_name_views_data() {
  $data = array();

  $data['module_name']['table']['group'] = t('entity_title');

  $data['base_table']['table']['base'] = array(
    'title' => t('entity_title'),
    'help' => t('module_entity_table_help_comment'),
    'field' => 'primary_key',
  );
//aao_ppgm_speaker refers to the name of the entity  not the table name in this case because they have the same name
//addRelationshipsHere

//addViewHandlerFields

  return $data;
}