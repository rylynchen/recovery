<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class recovery_handler_edit_link_field extends recovery_handler_link_field {
  
  function construct() {
    parent::construct();
  }

  function render($values) {
    // Check access
    if (!user_access('edit recovery')) {
      return;
    }
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $rid = $values->{$this->aliases['rid']};
    return l($text, 'recovery/' . $rid . '/edit');
  }
}
