<?php

class Product_Category {

/*
   * this should return categories in nested order
   * TODO must fix by adding a query to recursively
   * search and get children for a parent that has children
   * 
   * make a form element called categories
   */
  public function create_category_checkboxes($categories, &$form, $component, $frontend = false, $selected_values = array()) {
    $form['categories'] =array(
      "#type" => "checkboxes",
      "#title" => "select categories to display",
      '#options' => null,
      "#collapsible" => TRUE,
      "#collapsed" => TRUE,
    );
    foreach($categories as $item) {
      $current_level = ($item['level'])-1;
      if($item['children_count'] == 0) { //TODO -seems redundant remove default a var to parent and do an if and then one category level create call
        $this->category_level_create($form, $item, "child", $component, $frontend, $selected_values);          
      } else {
        $this->category_level_create($form, $item, "parent", $component, $frontend, $selected_values);
      }
    }
    return $form;
  } 

  /**
   * create a multidimensional array for a given
   * level (array[x][x+1]...[x+(n-1)][x+n])
   * $item['level, $item['parent_id, $item['id
   * @param form
   *  drupal form array
   * @param item
   *  category object
   * @param type
   *  string parent or child
   * @param component
   *  the componentfrom the form an array
   * 
   * @return form
   * an array
   */
  public function category_level_create(&$form, $item, $type, $component, $frontend = false, $selected_values) {
    $default_value = 0;
    if(array_key_exists($item['id'], $selected_values) || array_search($item['id'], $selected_values)) {
      if($selected_values[$item['id']]==1) {
        $default_value = 1;
      }
    } 
    if($type == "parent") {
      $form['categories'][$item['id']] =array(
        "#type" => "checkbox",
        "#title" => $item['name'],
        "#prefix" => "<div class='category_select_"
          . $item['id'] . " parent_category_" . $item['parent_id'] 
          . "' style='margin-left:" . (($item['level'])*6) . "px;'>",
        //"#parents" => array("submitted","profile_information_header", "venue_sweets_categories","categories",$item['id),
        "#suffix" => "</div>",
        "#default_value" => $default_value,
        '#description' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
        );    //"#field_prefix" => "<a class='child_category_select'></a>"<img src='http://aaortho.net/js/spacer.gif' />

    } elseif($type=="child") {
      $form['categories'][$item['id']] =array(
        "#type" => "checkbox",
        "#title" => $item['name'],
        "#prefix" => "<div class='category_select_"
          . $item['id'] . " parent_category_" . $item['parent_id'] 
          . "' style='margin-left:" . (($item['level'])*6) . "px;'>",
        "#suffix" => "</div>",
        "#default_value" => $default_value,
        '#description' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
        //"#parents" => array("submitted","profile_information_header", "venue_sweets_categories","categories", $item['id),
        );    //"#field_prefix" => "<a class='child_category_select'><img src='http://aaortho.net/js/spacer.gif' /></a>"
    } else {
      
    }
    return $form;
  }
  /**
   * get categories in order of parent and child
   * @param categories
   *  array of category objects
   * @return sorted_categories
   *  array of sorted categories
   */
  public function getSortedCategories($categories) {
    $sorted_categories = array();
    $i = 0;
    $i = 0;
    $j = 0;
    $categories = $categories->fetchAll();
    $num_categories = count($categories);
    for($i = 0; $i < $num_categories; $i++) {
      if(isset($categories[$i]) && $categories[$i]->level == 2) {
        $sorted_categories[$j]['id'] = $categories[$i]->id;  
        $sorted_categories[$j]['name'] = $categories[$i]->name;
        $sorted_categories[$j]['level'] = $categories[$i]->level;
        $sorted_categories[$j]['position'] = $categories[$i]->position;
        $sorted_categories[$j]['parent_id'] = $categories[$i]->parent_id;
        $sorted_categories[$j]['children_count'] = $categories[$i]->children_count;
        $parent_cat_check = array($categories[$i]->id); //store all child categories here and if any child 
        //parent category matches this then that means it is a great grand child of the parent
        $j++;
        for($k = 7; $k < $num_categories; $k++) {
          if(in_array($categories[$k]->parent_id, $parent_cat_check)) { //is connected to parent cat
            $parent_cat_check[] = $categories[$k]->id;
            $sorted_categories[$j] = array(
              'name' => $categories[$k]->name,
              'level' => $categories[$k]->level,
              'position' => $categories[$k]->position,
              'id' => $categories[$k]->id,
              'parent_id' => $categories[$k]->parent_id,
              'children_count' => $categories[$k]->children_count,
            );
            $j++;
          }
        }
      }
    }
    return $sorted_categories;
  }
   /**
    * returns all categories
    * @param approved_ids
    *   category ids to select
    * @param excluded_categories
    *   category ids not to select
    * @return array 
    *   an array of objects
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
      $query->orderBy('level', 'ASC')->orderBy('position', 'ASC');
    $result = $query->execute();
    if($result) {
      return $result;
    } else {
      return false;
    }
   }
}
