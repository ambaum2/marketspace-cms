<?php

class magento_product_category extends magento {
    
  /*
   * create the category tree for admin selection or 
   * frontend user if $approved_list is set
   * 
   * $approved_list array of categories to display
   */  
  public function create_category_select($categories, $approved_categories = array()) {
    $form_options = array(); 
    foreach($categories as $item) {
      $form_options[$item->id] = $item->name;
      if($item->children_count > 0) {
        $this->display_category_tree($approved_categories);  
      }
    }
    return $form_options;
  } 
  /*
   * this should return categories in nested order
   * TODO must fix by adding a query to recursively
   * search and get children for a parent that has children
   */
  public function create_category_checkboxes($categories, &$form, $approved_categories = array()) {
    foreach($categories as $item) {
      $current_level = ($item->level)-1;
      if($item->children_count == 0) {
          //$result = $this->get_category_first_or_last($item->id, $item->parent_id, "DESC");
          
          $form[$item->level - 1][$item->parent_id][$item->name . "_" . $item->id] = array(
            '#type' => "checkbox",
            '#title' => $item->name,
            '#prefix' => "<div style='margin-left:" . (($item->level)*25) . "px;'>",
            '#suffix' => "</div>",
          );
      /*} elseif($item->level == 2) {
          $form[$item->name . "_" . $item->id] = array(
            '#type' => "checkbox",
            '#title' => $item->name,
            '#prefix' => "<div style='margin-left:" . (($item->level)*25) . "px;'>",
            '#suffix' => "</div>",
          );*/
      } else {
        $array_string = "";
        //$arr = array(, array(2));
        echo "<pre>" . print_r($arr,true) . "</pre>";
        //create multi dim array for a level
        for($i=1;$i<4;$i++) {
          $array_string .= '['. $i . ']';
        }
        //
        eval('$thearray' . $array_string . '=array(1);');
        echo $array_string;
        $thearray = array();
        
        echo "<pre>" . print_r($thearray,true) . "</pre>";
        echo (20/.75);
        //echo $item->level;
        //  $depth = 5;
        //eval('$array = '.str_repeat('array(', $depth).str_repeat(')', $depth).';');
        //print_r($array);
          //eval('$build_array = ' . str_repeat('array(', $item->level).str_repeat(')', $item->level).';');
          //print_r($build_array);
          die;
          $form[$item->level][$item->name . "_" . $item->id] = array(
            '#type' => "checkbox",
            '#title' => $item->name,
            '#prefix' => "<div class='parent_category' style='margin-left:" . (($item->level))*25 . "px;'>",
            '#suffix' => "</div>", //style='margin-left:" . (($item->level))*25 . "px;'
            '#field_suffix' => "<a class='child_category_select'></a>",
          ); 
          $form[$item->level][$item->id] = array(
            '#type' => 'container',
            '#prefix' => "<div class='child_categories_container child_categories_hide' style='margin-left:" . (($item->level)*5) . "px;'>",
            '#suffix' => "</div>",
          );
      }
    }
    return $form;
  } 
  public function category_selector_add($content, $element) {
    if($element['#id'] != "edit-extra-magento-categories-experience-travel-packages-111") { 
    echo "<pre>" . print_r($element,true) . "</pre>";
    die;
    }
  }
  /*
   * get all categories for a store in magento
   */
  public function get_category_tree($session_id,$parent_id = 1, $store_view = 1,$host = parent::primary_host) {
    $proxy = new SoapClient($host);
    try {
      $result = $proxy->catalogCategoryTree($session_id, $parent_id, $store_view);
      $this->update_category_records($result);
      return $result;
    } catch (exception $e) {
      throw $e;
    }
  }
  /*
   * iterate through and update or insert
   * category records
   */
  public function update_category_records($tree) {
    foreach($tree->children as $cat) {
      if($this->exists_category($cat->category_id)) {
        $this->update_category($cat);
      } else {
        $this->insert_category($cat);
      }
      if(count($cat->children)>0) {
        $this->update_category_records($cat);
      }
    }
  }
  /*
   * 
   */
  public function display_category_tree($categories, $approved_categories = array()) {
    echo "<style type='text/css'>ul li {float:left;margin-left:20px;}</style>";
    //echo "<pre>" . print_r($categories,true) . "</pre>";
    foreach($categories as $cat) {
      echo "<br /><ul><li style='margin-left:" . (($cat->level)*20) . "px;'>" . $cat->name . "</li><li>Parent" . $cat->parent_id
        . "</li><li>position: " . $cat->position . "</li><li>level: " . $cat->level
        . "</li><li>children count: " . count($cat->children) . "</li></ul><br />";
      if($cat->children_count > 0) {
        $this->display_category_tree($approved_categories);  
      }
    }
  }
  /*insert category into webform_es_category table
   * 
   */
    private function insert_category($category_node) {
    $id = db_insert('webform_es_category')
      ->fields(array(
        'id' => $category_node->category_id,
        'parent_id' => $category_node->parent_id,
        'name' => $category_node->name,
        'is_active' => $category_node->is_active,
        'position' => $category_node->position,
        'level' => $category_node->level,
        'children_count' => count($category_node->children)
      ))
      ->execute();
   }
    /*
   * update  data for webform_es_category
   * @TODO - put db functions into own class as 
   */
   private function update_category($category_node) {
    $id = db_update('webform_es_category')
      ->fields(array(
        'id' => $category_node->category_id,
        'parent_id' => $category_node->parent_id,
        'name' => $category_node->name,
        'is_active' => $category_node->is_active,
        'position' => $category_node->position,
        'level' => $category_node->level,
        'children_count' => count($category_node->children)
      ))
      ->condition('id', $category_node->category_id)
      ->execute();
   }
   /*
   * see if record exists into webform_es_category
   * @TODO - put db functions into own class as 
   */
   private function exists_category($category_id) {
    $query = db_select('webform_es_category', 'wec')
      ->fields('wec',array('id'))
      ->condition('id',$category_id);
    $result = $query->execute()->fetch();
    if($result) {
      return true;
    } else {
      return false;
    }
   }
   
   private function get_stored_category($category_id) {
    $query = db_select('webform_es_category', 'wec')
      ->fields('wec',array('id','parent_id','name','position','level','children_count'))
      ->condition('id',$category_id);
    $result = $query->execute()->fetch();
    if($result) {
      return $result;
    } else {
      return false;
    }
   }
   /*
    * returns all categories
    * TODO - should probably sort by level and then position
    */
   public function get_categories($approved_ids = array(),$excluded_categories = array(2)) {
    $query = db_select('webform_es_category', 'wec')
      ->fields('wec',array('id','parent_id','name','position','level','children_count'))
      ->condition('is_active', 1);
      if(count($approved_ids) > 0) {
        $query->condition('id',$approved_ids, 'IN');
      }
      if(count($excluded_categories) > 0) {
        $query->condition('id',$excluded_categories, 'NOT IN');
      }
    $result = $query->execute();
    if($result) {
      return $result;
    } else {
      return false;
    }
   }
  /*
   * check if the current category is last (ASC) or first (DESC)
   * parent id 
   * sort by position desc limit 1
   * default search for last
   */
  public function get_category_first_or_last($id, $parent_id, $position = "ASC") {
    $query = db_select('webform_es_category', 'wec')
      ->fields('wec',array('id','name','position','level','children_count'))
      ->condition('parent_id',$parent_id)
      ->orderBy('position', $position)
      ->range(0,1);
    $result = $query->execute()->fetch();
    if($result) {
      return $result;
    } else {
      return false;
    }
   }
}