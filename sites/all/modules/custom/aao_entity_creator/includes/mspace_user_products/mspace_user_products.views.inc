<?php

/**
 * Implements hook_views_data().
 */
function mspace_user_products_views_data() {
  $data = array();

  $data['mspace_user_products']['table']['group'] = t('Marketspace User Products');

  $data['mspace_user_products']['table']['base'] = array(
    'title' => t('Marketspace User Products'),
    'help' => t('assigns a user to a product. multiple users possible'),
    'field' => 'id',
  );
//aao_ppgm_speaker refers to the name of the entity  not the table name in this case because they have the same name
//addRelationshipsHere

  $data['mspace_user_products']['id'] = array(
    'title' => t('Id'),
    'help' => t('unique id of this product record'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );
  $data['mspace_user_products']['uid'] = array(
    'title' => t('Uid'),
    'help' => t('drupal user id. maps back to magentos marketspace owner field'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );
  $data['mspace_user_products']['product_id'] = array(
    'title' => t('Product Id'),
    'help' => t('product id from magento'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );


  return $data;
}