$(function () {

    let $preview = $('#map-preview'),
        $map_content = $('#map-contents');

    $preview.hide();

    $.getScript('/static/js/game.js')
    .done(function() {
        let game = create_game();
        let map = $map_content[0].textContent.replaceAll('\n', '');
        game.addToDOM($preview[0]);
        game.displaySimpleMap(map);
        // reveal the UI
        $preview.fadeIn('fast');
    });
});
