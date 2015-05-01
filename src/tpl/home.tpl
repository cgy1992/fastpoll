<?php

const ANSW_INIT = 4;

?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <?php include "bits/meta.tpl" ?>
    <title>FastPoll - Create new poll</title>
  </head>
  <body>
    
    <?php include "bits/header.tpl" ?>
    
    <main role="main" id="main">
      <div class="page">
        <h2>Create new poll</h2>
        <form class="form cbox form--poll" action="poll" 
          method="post" data-answ="<?= ANSW_INIT ?>">
          <div class="view">
            <div class="fbox">
              <label for="poll_title">Title:</label>
              <textarea id="poll_title" placeholder="..."></textarea>
            </div>
            <?php for ($i = 0; $i < ANSW_INIT; ++$i): ?>
              <div class="fbox answ">
                <label for="poll_answ<?= $i ?>">Answer #<?= $i + 1 ?>:</label>
                <input type="text" class="text" placeholder="..."
                  id="poll_answ<?= $i ?>" name="answ[<?= $i ?>]">
              </div>
            <?php endfor; ?>
            <div class="more"></div>
            <div class="fbox">
              <input type="button" class="btn btn--semi add" value="Add field">
              <p class="desc">Up to 100 fields per poll</p>
            </div>
            <div class="fbox">
              <label>
                <input type="checkbox">
                Allow multiple choice
              </label>
              <label>
                <input type="checkbox">
                Allow duplicate votes (from same IP-Address)
              </label>
            </div>
          </div>
          <div class="ftrl">
            <input type="submit" class="btn btn--main" value="Create">
          </div>
        </form>
      </div>
    </main>
    
    <?php include "bits/footer.tpl" ?>
    
    <script src="js/main.js" defer></script>
  </body>
</html>
