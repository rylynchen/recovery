<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class RecoveryMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['uid'] = array(
      'label' => t('Uid'),
      'description' => t('The recovery uid.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer recovery',
      'schema field' => 'uid',
    );
    $properties['type'] = array(
      'label' => t('Type'),
      'description' => t('The recovery type.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer recovery',
      'schema field' => 'type',
    );
    $properties['code'] = array(
      'label' => t('Code'),
      'description' => t('The recovery code.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer recovery',
      'schema field' => 'code',
    );
    $properties['created'] = array(
      'label' => t('Created'),
      'description' => t('The recovery created.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer recovery',
      'schema field' => 'created',
    );
    $properties['expiry'] = array(
      'label' => t('Expiry'),
      'description' => t('The recovery expiry.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer recovery',
      'schema field' => 'expiry',
    );
    $properties['status'] = array(
      'label' => t('Status'),
      'description' => t('The recovery status.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer recovery',
      'schema field' => 'status',
    );
    return $info;
  }
}