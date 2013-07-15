<?php

/**
 * Implements hook_views_data()
 */
function recovery_views_data_alter(&$data) {
  $data['recovery']['link_recovery'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the recovery.'),
      'handler' => 'recovery_handler_link_field',
    ),
  );
  $data['recovery']['edit_recovery'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the recovery.'),
      'handler' => 'recovery_handler_edit_link_field',
    ),
  );
  $data['recovery']['delete_recovery'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the recovery.'),
      'handler' => 'recovery_handler_delete_link_field',
    ),
  );
  $data['recovery']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this recovery.'),
      'handler' => 'recovery_handler_recovery_operations_field',
    ),
  );
}