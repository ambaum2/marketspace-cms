<?php
require 'PHPUnit/Autoload.php';
class magento_product_categoryTest extends PHPUnit_Framework_TestCase
{
  public $dpl_dir = '/var/www/mspacecms/';
  const DRUPAL_ROOT = '/var/www/mspacecms/';
  const REMOTE_ADDR = '162.243.110.111';
  
  public function __construct() {
    $_SERVER['HTTP_HOST'] = basename($this->dpl_dir);
    $_SERVER['REMOTE_ADDR'] = '127.0.0.1';  
    define('DRUPAL_ROOT', $this->dpl_dir);
    set_include_path($this->dpl_dir . PATH_SEPARATOR . get_include_path());
    require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);    
  }
  public function testgetAllCategories() {
    $category = new magento_product_category;
    $catFields = array('id','parent_id','name','position','level','children_count');
    $categories = $category->get_categories($checked_categories, $excluded_categories = array(2));
    $cats = array();
    $i = 0;
    $i = 0;
    $j = 0;
    $total_iterations = 0;
    $categories = $categories->fetchAll();
    $num_categories = count($categories);
    for($i = 0; $i < $num_categories; $i++) {
      $total_iterations++;
      if(isset($categories[$i]) && $categories[$i]->level == 2) {
        $cats[$j]['id'] = $categories[$i]->id;  
        $cats[$j]['name'] = $categories[$i]->name;
        $cats[$j]['level'] = $categories[$i]->level;
        $cats[$j]['position'] = $categories[$i]->position;
        $cats[$j]['parent_id'] = $categories[$i]->parent_id;
        $cats[$j]['children_count'] = $categories[$i]->children_count;
        $parent_cat_check = array($categories[$i]->id); //store all child categories here and if any child 
        //parent category matches this then that means it is a great grand child of the parent
        $j++;
        for($k = 7; $k < $num_categories; $k++) {
          if(in_array($categories[$k]->parent_id, $parent_cat_check)) { //is connected to parent cat
            $parent_cat_check[] = $categories[$k]->id;
            $cats[$j] = array(
              'name' => $categories[$k]->name,
              'level' => $categories[$k]->level,
              'position' => $categories[$k]->position,
              'id' => $categories[$k]->id,
              'parent_id' => $categories[$k]->parent_id,
              'children_count' => $categories[$k]->children_count,
            );
            $j++;
          }
          $total_iterations++;
        }
      }
    }
    $this->assertEquals($num_categories, count($cats));
    print "\n total iterations: $total_iterations";
    //$category->create_category_checkboxes($categories, $form, $component, true, $selected_categories);
  }
  public function testcreate_category_checkboxes() {
    $category = new Product_Category;
    $categories = $category->get_categories($approved_ids = array(),$excluded_categories = array(2)); 
    $categories = $category->getSortedCategories($categories);
    $category->create_category_checkboxes($categories, $form, $component);
  }
}
