<?php

interface iMS_CRM {
    public function get(); /** get a resource(s) */
    public function post(); /** create a new resource with an unkown location */
    public function put(); /** modify or create a resource with a known location*/
    public function delete(); /** delete a resource */
    public function patch(); /** update one field */
}