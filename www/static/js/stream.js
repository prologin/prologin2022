async function load_next_interesting() {
    console.log("downloading next interesting match");
    //const data = await (await window.fetch('/api/matches/interesting/')).json();
    const data = await (await window.fetch('/api/matches/interesting/')).json();
    const response = await fetch(data.dump_url);
    if (response.status === 200) {
        let data = await response.text();
        data = data.substring(0, data.length - 1).replaceAll('\n', ',');
        const dump_data = JSON.parse(`{"dump": [${data}]}`)["dump"];
        console.log('dump loaded');
        console.log(dump_data);
        return dump_data;
    }
    return []
}


$(function() {
    let $replay = $('#replay');
    let $replay_view = $replay.append('<div id="replay_view"></div>');

    $.getScript('/static/js/pixi.min.js').done(function() {
        $.getScript('/static/js/game.js')
        .done(function() {
            let game = create_game();
            game.addToDOM($replay_view[0]);
            get_ticker().autoStart = false;
            for (let i = 0; i < 32; i++) {
                get_ticker().add(game.gameLoop, game);
            }
            (async function () {
              while(true) {
                const dump = await load_next_interesting();
                game.setupGame(dump);
                await game.stream();
              }
            })();
        })
    });
});
