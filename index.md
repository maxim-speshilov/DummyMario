---
layout: default
youtube_id: DpmfwWbonVY
---

![IT'S-A ME, MARIO](/assets/img/its_mario.png)

### Go!

```c++
while (window_.isOpen()) {
    float dt = clock.restart().asSeconds();
    time += dt;
	
    while (time > time_per_frame_) {
        time -= time_per_frame_;
        
        processInput();
        update(time_per_frame_);

        if (state_stack_.isEmpty())
            window_.close();
    }

    render();
}
```

### Gameplay

{% include youtube_player.html id=page.youtube_id %}

<br/><br/>
