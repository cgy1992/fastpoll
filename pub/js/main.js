/*!
 * FasterPoll - FastCGI Poll Service
 * @copyright Copyright (c) 2015, The FasterPoll authors
 */

;(function() {
  'use strict';
  
  var win = window,
      doc = document;
      
  /* ------------------------------------ */
  
  var MAX_FIELDS = 100;
  
  /* ------------------------------------ */
  /* language functions */
  
  var lang = win.i18n || {};
  
  /**
   * gettext-look-alike
   * @param  {String} key
   * @return {String}
   */
  function _(key) {
    if (typeof lang[key] !== 'undefined')
      return lang[key];
    return key;
  }
  
  /**
   * "create new poll" form setup
   */
  (function() {
    var fp = doc.querySelector('.form--poll'),
        id, mc, ab, ti = 0;
    if (!fp) return; // no poll form
    id = fp.dataset.answ|0;
    mc = fp.querySelector('.more'); // additional answers container
    ab = fp.querySelector('.btn.add'); // add-button
    /**
     * adds a new form-field
     */
    function addField() {
      if (id >= MAX_FIELDS) return;
      var el = doc.createElement('div'),
          lb = doc.createElement('label'),
          ip = doc.createElement('input');
      // setup container
      el.classList.add('fbox');
      el.classList.add('answ');
      // setup label
      lb.setAttribute('for', 'poll_answ' + id /* zero based */);
      lb.textContent = 'Answer #' + (id + 1) + ':';
      // setup checkbox
      ip.setAttribute('type', 'text');
      ip.classList.add('text');
      ip.setAttribute('id', 'poll_answ' + id /* zero based */);
      ip.setAttribute('placeholder', 'Enter option here ...');
      // combine
      el.appendChild(lb);
      el.appendChild(ip);
      mc.appendChild(el);
      // update form
      fp.dataset.answ = ++id;
      // disable button if necessary
      ab.disabled = id >= MAX_FIELDS;
    }
    /**
     * checks if a new form-field should be added
     * and adds one if necessary
     */
    function maybeAddField() {
      var af = fp.querySelectorAll('.answ input');
      // noting found? add a field ...
      if (!af || af.length === 0) return true;
      for (var i = 0, l = af.length; i < l; ++i)
        if (af[i].value === '') return;
      addField();
    }
    /**
     * adjusts the form
     */
    function adjustForm() {
      win.cancelAnimationFrame(ti);
      ti = win.requestAnimationFrame(maybeAddField);
    }
    // setup form
    fp.addEventListener('keyup', adjustForm); // less frequent than keydown
    fp.addEventListener('paste', adjustForm);
    adjustForm(); // directly
    // setup add-button
    (function() {
      var af = fp.querySelectorAll('.answ input');
      ab.disabled = af && af.length >= MAX_FIELDS;
      ab.addEventListener('click', addField);
    })();
  })();
  
})();

