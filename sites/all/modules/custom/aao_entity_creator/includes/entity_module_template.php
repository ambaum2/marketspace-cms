<?php
/*
 *replace: module_name, module_title, module_primary_key,
 * module_primary_key_label, module_base_table, module_url_key
 * 
 */
/**
 * @file
 * Provides a simple custom entity type named 'ppgm' for the preview program.
 */

/**
 * Implements hook_entity_info().
 */
function module_name_entity_info() {
  $info = array();
  $info['entity_machine_name'] = array(
    // Human readable label.
    'label' => t('entity_title'),
    'base table' => 'module_base_table',
    'entity keys' => array(
      'id' => 'entity_primary_key',
      'label' => 'entity_primary_key_label',
    ),
    'uri callback' => 'entity_class_uri',
    // This is the default controller.
    //'controller class' => 'DrupalDefaultEntityController',
    // Other options provided by the EntityAPI contrib module which we'll be using later.
    // 'controller class' => 'EntityAPIController ',
    // 'controller class' => 'EntityAPIControllerExportable',
    'entity class' => 'module_name',
    // This is a uri function provided by the Entity contrib module.
    // It's a simple wrapper around the uri method that needs to be implemented in the controller class.
    'uri callback' => 'entity_class_uri',
    // Here is our custom controller that we will provide below.
    'controller class' => 'module_nameEntityController',
         // The information below is used by the VideoEntityUIController (which extends the EntityDefaultUIController)
     'admin ui' => array(
       'path' => 'admin/module_name',
       'controller class' => 'module_nameEntityUIController',
       'menu wildcard' => '%module_name',
       'file' => 'module_name.admin.inc'
     ),
    'module' => 'module_name',
    //Acess callback to determine permissions
    'access callback' => 'module_name_access_callback',
   ); 
   
   return $info;
 }

 /**
 * Implements hook_menu().
 */
function module_name_menu() {
  $items = array();
    $items['module_name/%module_url_key'] = array(
    'title' => 'module_title',
    'page callback' => 'module_name_view_entity',
    'page arguments' => array(1),
    'access callback' => TRUE,
  );
  return $items;
}
/**
 * Implements hook_views_api().
 */
function module_name_views_api() {
  return array(
    'api' => 3,
    'path' => drupal_get_path('module', 'module_name'),
  );
}
/**
 * Callback for /module_name page.
 *
 * Just a place to test things out and be able to see the results on a page.
 */
function module_name_demo_page() {
  $module_name = entity_load('module_name', array(1));
 kpr($module_name);
 $content = "hello!";
   // Or load it with EFQ.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'module_name');
  $results = $query->execute();
 kpr($results);
  return $content;
}

/**
 * Implements hook_entity_property_info().
 */
function module_name_entity_property_info() {
  $info = array();
  $properties = &$info['module_name']['properties'];
  
//addPropertyItemsPlaceHolder
  
  return $info;
}
/**
 * Implements hook_permission().
 */
function module_name_permission() {
  return array(
    'administer module_name' => array(
      'title' => t('Administer module_name Entities'),
    ),
    'view module_name' => array(
      'title' => t('View module_name Entities'),
    ),
    );
  }
/**
 * Check access permissions for module_name entities
 */
function module_name_access_callback($op, $module_name = NULL, $account = NULL){
  if(($op == 'view' && user_access('view module_name', $account)) || user_access('administer module_name', $account)){
    return TRUE;
  }
  else if (user_access('administer module_name', $account)){
    return TRUE;
  }
  return TRUE;
}
/**
 * 
 * /
 * Menu autoloader for /module_name
 */
 function module_name_load($id) {
  $module_name = entity_load('module_name', array($id));
  return array_pop($module_name);
 }
/**
 * Callback for /video/ID page.
 *
 * Just a place to render a complete video entity.
 */
function module_name_view_entity($module_name) {
  drupal_set_title($module_name->name);
  $module_name_entity = entity_view('module_name', array($module_name->id => $module_name), 'member');
  kpr($module_name);
  return $module_name_entity;
}

/**
 * Our custom entity class.
 */
class module_name extends Entity {
  /**
   * Override this in order to implement a custom default URI.
   */
  protected function defaultUri() {
    return array('path' => 'ppgm/' . $this->identifier());
  }
}

/**
 * Our custom controller for the videoentity type.
 *
 * We're choosing to extend the controller provided by the entity module so that we'll have
 * full CRUD support for videoentities.
 */
class module_nameEntityController extends EntityAPIController {

} 

/**
 * Our custom controller for the admin ui.
 */
class module_nameEntityUIController extends EntityDefaultUIController {
  
}
