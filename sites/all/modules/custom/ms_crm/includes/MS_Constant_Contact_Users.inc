<?php

/**
 * meant to handle the drupal integration
 * whenever a new contact is added
 * Class MS_Constant_Contact_Users
 */
class MS_Constant_Contact_Users{
    public $name;
    public $mail;
    public $password;
    public $init;
    public $status;
    public $roles; /** @var $roles array of role ids */

    /**
     *
     */
    public function CreateUser() {
        $user = new Entity();
    }
}