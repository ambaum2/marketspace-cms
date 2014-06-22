<?php

class MS_Constant_Contact_Admins {
    /**
     * @return mixed
     */
    public function GetEventRecipients() {
        $query = 'SELECT DISTINCT(ur.uid), u.mail
          FROM {users_roles} AS ur
          LEFT JOIN {users} AS u on u.uid = ur.uid
          WHERE ur.rid IN (:rids) and u.status = 1';
        $result = db_query($query, array(':rids' => array(3)));

        return $result->fetchAllKeyed();
    }
}