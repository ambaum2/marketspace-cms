<?php
class makeEntityFiles {
  /*$database = "mspacecms";
  $module_name = "aao_lectures";
  $entity_title = "Lectures Module"; //will be schemea and entity name
  $entity_machine_name = "aao_lectures";
  $base_table = "cache";
  $primary_key = "id";
  $primary_key_label = "lecture_id";
  $module_url_key = "lectures";
  $tabs = array("  ", "  ", "    ", "      ", "        ");
  
  $settings = array();
  $settings['module_name'] = $module_name;
  $settings['entity_title'] = $entity_title;
  $settings['base_table'] = $base_table;
  $settings['primary_key'] = $primary_key;
  $settings['module_entity_table_help_comment'] = "this table is for something";
  //remove old files if they exist
  shell_exec("rm -rf " . $module_name . ".install");
  shell_exec("rm -rf " . $module_name . ".module");
  shell_exec("rm -rf " . $module_name . ".views.inc");
  $mysqli = new mysqli("localhost", "root", "foley33$", $database);
  if ($mysqli->connect_errno) {
      echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
  }
  
  echo $mysqli->host_info . "\n";
  if(!$result = $mysqli->query("SHOW FULL COLUMNS FROM " . $base_table . ";")) {
    echo "failed to show tables";
  }
  //create install file
  createEntityInstallFile($mysqli->query("SHOW FULL COLUMNS FROM " . $base_table . ";"), $module_name, $entity_machine_name, $tabs);
  //create module file
  createEntityModuleFile($mysqli->query("SHOW FULL COLUMNS FROM " . $base_table . ";"), $module_name, $entity_machine_name, $entity_title, $base_table, $primary_key, $primary_key_label, $module_url_key, $tabs);
  //create views file
  createEntityViewsFile($mysqli->query("SHOW FULL COLUMNS FROM " . $base_table . ";"), $settings, $tabs);
  //create info file 
  createModuleInfoFile($settings);*/
  /**
   * checks the string for an array of types and
   * converts mysql type field to schema
   * type for drupal
   * @TODO if something is serial that means its a primary key
   * for sure but I don't know any other common scenarios so leaving
   * @todo also leaving out "tinytext"=>array("int", "small") for
   * now b/c there are two tinytext sizes
   * @todo i don't have any good float examples so only using normal
   * float
   * it out for now except when Key is PRI
   * @param data | array
   *  array of the mysql schemea row
   * @return array
   *  returns an array keyed with type and size or length for varchar
   */
  function convert_mysql_type($data) {
    $info = array();
    $type = $data['Type'];
    $schema_map = array("tinyint"=>array("int", "tiny"), "smallint"=>array("int", "small"), "mediumint"=>array("int", "medium"),
      "bigint"=>array("int", "big"), "int"=>array("int", "normal"),"tinytext"=>array("text", "small"), 
      "mediumtext"=>array("text", "medium"), "bigtext"=>array("text", "longtext"), "text"=>array("text", "normal"), 
      "varchar"=>array("varchar", "normal"), "char"=>array("char", "normal"), "numeric"=>array("numeric", "normal"), 
      "longblob"=>array("blob", "big"), "float"=>array("float", "normal"), "double"=>array("float", "big"),
      "datetime"=>array("datetime", "normal"), "serial"=>array("serial", "normal"), "blob"=>array("blob", "normal"));
    if(strpos($type, "(")) { // only care about type name for these non varchar adn datetime ones
      $string = explode("(", $type); //should be
      if(array_key_exists($string[0], $schema_map)) {
        $info['type'] = $schema_map[$string[0]][0];
        if(strpos($type, "varchar") || strpos($type, "char")) {
          $info['length'] = substr($string[1], 0, (strpos($type, ")")) - (strpos($type, "(")) - 1); 
        } else {
          $info['size'] = $schema_map[$string[0]][1];
        }
      } 
      if(strpos($type, "unsigned"))
        $info['unsigned'] = "TRUE";
    } else {
      if(array_key_exists($type, $schema_map)) {
        $info['type'] = $schema_map[$type][0];
        if(strpos($type, "datetime")) 
          $info['mysql_type'] = "DATETIME";
      } else {
        $info['type'] = 'text';
        $info['size'] = 'normal';      
      }
    }
    if($data["Key"] == "PRI") {
      $info['type'] = 'serial';
      unset($info['size']);
    }
    return $info; 
  }
  /**
   * create install file
   * 
   * @param result | resource
   *  resource from database query which is 
   *  the msyql schema for a table
   * @param module_name | string
   *  name of module to create
   * @param entity_name | string
   *  machine name of entity to create
   * @param tabs | array
   *  an array of strings with different lengths 
   * of spaces array("  ", "    ", etc) so I didn't 
   * have to manually put in spaces and not use
   * /t which tabs too long
   * @todo remove all the fwrites they are not
   * efficient. Use file_get_contents and 
   * file_put_contents instead.
   */
  function createEntityInstallFile($result, $module_name, $entity_name, $tabs) {
    $primary_keys = array();
    $fh = fopen(drupal_get_path('module', 'aao_entity_creator') . '/includes/' . $module_name . '/'. $module_name . ".install", "a");
    drupal_set_message("file made?");
    //comment the code
    fwrite($fh, "<?php" . PHP_EOL);
    fwrite($fh, "/**" . PHP_EOL);
    fwrite($fh, "* Implements hook_schema()." . PHP_EOL);
    fwrite($fh, "*/" . PHP_EOL);
    //begin the function
    fwrite($fh, "function {$module_name}_schema() {" . PHP_EOL);
    fwrite($fh, "{$tabs[0]}\$schema = array();" . PHP_EOL); //declare schemea array to be returned
    fwrite($fh, "{$tabs[0]}\$schema['{$entity_name}'] = array(" . PHP_EOL); //create entity
    fwrite($fh, "{$tabs[2]}'description' => 'the base table for the {$entity_name} entity'," . PHP_EOL);
    fwrite($fh, "{$tabs[2]}'fields' => array(" . PHP_EOL);
    while($row = $result->fetchAssoc()) {
      fwrite($fh, "{$tabs[3]}'" . $row['Field'] . "' => array(" . PHP_EOL); //make field
      isset($row['Comment']) ? fwrite($fh, "{$tabs[4]}'description' => '" . utf8_encode($row['Comment']) . "'," . PHP_EOL) : ''; //comment
      $info = $this->convert_mysql_type($row);
      isset($info['type']) ? fwrite($fh, "{$tabs[4]}'type' => '" . $info['type'] . "'," . PHP_EOL) : ''; //type
      isset($info['size']) ? fwrite($fh, "{$tabs[4]}'size' => '" . $info['size'] . "'," . PHP_EOL) : ''; //size
      isset($info['length']) ? fwrite($fh, "{$tabs[4]}'length' => '" . $info['length'] . "'," . PHP_EOL) : ''; //length only varchar i think
      isset($info['mysql_type']) ? fwrite($fh, "{$tabs[4]}'mysql_type' => '" . $info['mysql_type'] . "'," . PHP_EOL) : ''; //length only datetime i think
      isset($info['unsigned']) ? fwrite($fh, "{$tabs[4]}'unsigned' => '" . $info['unsigned'] . "'," . PHP_EOL) : ''; //unsigned
      if(isset($row['Null'])) {
        ($row['Null'] == "NO") ? $allow_null = 'TRUE' : $allow_null ='FALSE';
        fwrite($fh, "{$tabs[4]}'not null' => '" . $allow_null . "'," . PHP_EOL); //unsigned
      }
      fwrite($fh, "{$tabs[3]})," . PHP_EOL); //end of schema column definition
    }
    
    //output any primary keys if $primary_key array count > 0
    
    fwrite($fh, "{$tabs[2]})," . PHP_EOL); //end of fields array
    fwrite($fh, "{$tabs[0]});" . PHP_EOL); //end of schema array
    fwrite($fh, "{$tabs[0]}return \$schema;" . PHP_EOL);//return schema 
    fwrite($fh, "}" . PHP_EOL); //end of function  
  }
  
  /**
   * check if key exists in array and return info array if
   * it does
   * @param type | string 
   *  the name of the mysql type
   * @param schema_map | array
   *  map of drupal schema to mysql array keys
   * @return array
   *  array of type and possibly length or size
   * depending on the type
   */
  
  /**
   * creates an info file
   */
  
  //createModuleInfoFile
  /**
   * create the module file for entity
   * @param result | resource
   *  resource from database query which is 
   *  the msyql schema for a table
   * @param module_name | string
   *  name of module to create
   * @param entity_machine_name | string
   *  machine name of entity to create
   * @param entity_title | string
   *  human readable title
   * @param base_table | string
   *  name of base table in database
   * @param primary_key | string
   *  entities primary key field - only one 
   *  for now
   * @param primary_key_label | string
   *  Human readable label for entities
   *  primary key
   * @param module_url_key | string
   *  the url for the admin view for this
   *  module
   * @param tabs | array
   *  an array of strings with different lengths 
   * of spaces array("  ", "    ", etc) so I didn't 
   * have to manually put in spaces and not use
   * /t which tabs too long
   * 
   * @TODO - too many variables make an array instead
   * @TODO - multiple primary_keys?
   */
  function createEntityModuleFile($result, $module_name, $entity_machine_name, $entity_title, $base_table, $primary_key, $primary_key_label, $module_url_key, $tabs) {
    //copy the template
    $file_path = drupal_get_path('module', 'aao_entity_creator') . '/includes/' . $module_name . '/'.$module_name . ".module";
    if (!copy(drupal_get_path('module', 'aao_entity_creator') . '/includes/entity_module_template.php', $file_path)) {
        echo "failed to copy the file...\n";
    }
    
    //get the file string
    $file_contents = file_get_contents($file_path);
    //replace the following: module_name, entity_title, entity_primary_key, entity_primary_key_label, module_base_table, module_url_key
    $file_contents = str_replace('module_name', $module_name, $file_contents);
    $file_contents = str_replace('entity_title', $entity_title, $file_contents);
    $file_contents = str_replace('entity_machine_name', $entity_machine_name, $file_contents);
    $file_contents = str_replace('entity_primary_key', $primary_key, $file_contents);
    $file_contents = str_replace('entity_primary_key_label', $primary_key_label, $file_contents);
    $file_contents = str_replace('module_base_table', $base_table, $file_contents);
    $file_contents = str_replace('module_url_key', $module_url_key, $file_contents);
    
    $properties_text = "";
    while($row = $result->fetchAssoc()) {
      $properties_text .= $tabs[0] . "\$properties['". $row['Field'] . "'] = array(" . PHP_EOL;
      $properties_text .= $tabs[2] ."'label' => t('". ucwords(strtolower(str_replace(array("_"), " ", $row['Field']))) . "')," . PHP_EOL;
      $properties_text .= $tabs[2] ."'schema field' => '". $row['Field'] . "'," . PHP_EOL;
      $properties_text .= $tabs[2] ."'description' => '". utf8_encode($row['Comment']) . "'," . PHP_EOL;
      $properties_text .= $tabs[0] .");" . PHP_EOL;
    }
    $file_contents = str_replace('//addPropertyItemsPlaceHolder', $properties_text, $file_contents);
    file_put_contents($file_path, $file_contents);
  }
  
  /**
   * create entity views file
   * 
   * 
      typical format for field
       $data['aao_ppgm_lecture']['id'] = array(
      'title' => t('Unique ID'),
      'help' => t("Lecture ID that relates speaker and lecture"),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),    
    );
     
     sorts just add this after field
      'sort' => array(
        'handler' => 'views_handler_sort_date_time',
      ),
    
   */
  function createEntityViewsFile($result, $settings, $tabs) {
    $file_name = drupal_get_path('module', 'aao_entity_creator') . '/includes/' . $settings['module_name'] . '/' . $settings['module_name'] . ".views.inc";
    if (!copy(drupal_get_path('module', 'aao_entity_creator') . '/includes/entity_views_template.php', $file_name)) {
        throw new Exception("failed to copy the file... for views\n", 1);     
    }
    
    $replacement_strings = array('module_name', 'entity_title', 'base_table', 'primary_key', 'module_entity_table_help_comment');
    $sort_fields = array('datetime' => 'views_handler_sort_date_time'); //any fields that shoudl have the sort handler set
    //most should default to a sort hanlder i think
    $file_contents = file_get_contents($file_name);
  
    //replace replacement string variables
    foreach($replacement_strings as $item) {
      $file_contents = str_replace($item, $settings[$item] , $file_contents);
    }
    
    //add views handler fields from database schema result
    //addViewHandlerFields is the token
    $views_fields_text = "";
    while($row = $result->fetchAssoc()) {
      $views_fields_text .= $tabs[0] . "\$data['" . $settings['base_table'] . "']['" . $row['Field'] . "'] = array(" . PHP_EOL;
      $views_fields_text .= $tabs[2] . "'title' => t('" . ucwords(strtolower(str_replace(array("_"), " ", $row['Field']))) . "')," . PHP_EOL;
      $views_fields_text .= $tabs[2] . "'help' => t('" . utf8_encode($row['Comment']) . "')," . PHP_EOL;
      $views_fields_text .= $tabs[2] . "'field' => array(" . PHP_EOL;
      $views_fields_text .= $tabs[3] . "'handler' => '" . $this->getViewFieldHandlerByDbFieldType($row) . "'," . PHP_EOL;
      $views_fields_text .= $tabs[2] . ")," . PHP_EOL;
      if(array_key_exists($this->cleanMysqlTypeText($row['Type']), $sort_fields)) { //define any sort filters
        $views_fields_text .= $tabs[2] . "'sort' => array(" . PHP_EOL;
        $views_fields_text .= $tabs[3] . "'handler' => '". $sort_fields[$row['Type']] . "'," . PHP_EOL;
        $views_fields_text .= $tabs[2] . "),";
      }
      $views_fields_text .= $tabs[0] . ");" . PHP_EOL;
    }
    //echo "i took my eyees off you for a minute";
    $file_contents = str_replace("//addViewHandlerFields", $views_fields_text, $file_contents);
    file_put_contents($file_name, $file_contents);
    //@TODO -create relationship at this token //addRelationshipsHere
    /*
       $data['aao_ppgm_lecture']['table']['join'] = array(
      'aao_ppgm_speaker' => array(
        'left_field' => 'lecture_id',
        'field' => 'lecture_id',
      ),
    );
    //@TODO - add relationships too 
    /*
         'relationship' => array(
        'base' => 'aao_ppgm_speaker',
        'field' => 'lecture_id',
        'handler' => 'views_handler_relationship',
        'label' => t('Lecture'),
      ),*/
  }
  
  /**
   * match a msyql type with 
   * a handler
   * 
   * @return string 
   *  returns the handler type
   * @TODO - views_handler_field_date_time and
   * views_handler_sort_date_time are custom
   * handlers I made for datetime there is nothing 
   * comparable in views core that i know of so probably
   * should add it to this module
   */
  function getViewFieldHandlerByDbFieldType($data) {
    $type = $data['Type'];
    $handler_map = array("tinyint"=>array("views_handler_field_numeric"), "smallint"=>array("views_handler_field_numeric"),
      "mediumint"=>array("views_handler_field_numeric"), "bigint"=>array("views_handler_field_numeric"),
      "int"=>array("views_handler_field_numeric"),"tinytext"=>array("views_handler_field"), 
      "mediumtext"=>array("views_handler_field"), "bigtext"=>array("views_handler_field"), "text"=>array("views_handler_field"), 
      "varchar"=>array("views_handler_field"), "char"=>array("views_handler_field"), 
      "numeric"=>array("views_handler_field_numeric"), "longblob"=>array("views_handler_field"),
      "float"=>array("views_handler_field_numeric"), "double"=>array("views_handler_field_numeric"),
      "datetime"=>array("datetime", "views_handler_field_date_time"), "serial"=>array("views_handler_field_numeric"),
      "blob"=>array("views_handler_field"));
    $type = $this->cleanMysqlTypeText($type);
    if(array_key_exists($type, $handler_map)) {
      return $handler_map[$type][0];
    } else {
      throw new Exception("Error, the type does not exist type: {$type}", 1);
      //return "Warning: Failed to find array key";
    }
  }
  
  /**
   * clean up mysql Type field if it has
   * a length. it is mainly just removing
   * parens
   * @param type | string
   *  type string 
   * @return string
   *  returned cleaned text
   */
  function cleanMysqlTypeText($type) {
    if($pos = strpos($type, "(")) {
      $type = substr($type, 0, $pos); 
    }  
    return $type;
  }
  /**
   * 
   * @param fileName | string
   *  file to search
   * @param str | string
   *  string to search
   * @return integer
   *  return line number >= 0 if success -1 
   * if failed
   */
  function getLineWithString($fileName, $str) {
      $lines = file($fileName);
      foreach ($lines as $lineNumber => $line) {
          if (strpos($line, $str) !== false) {
              return $lineNumber;
          }
      }
      return -1;
  }
}