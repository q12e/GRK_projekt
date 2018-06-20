# GRK_projekt
* Ryby są tworzone losowo, podczas tworzenia ryb, dla każdej jest losowany kierunek poruszania się oraz wielkość.
* Wodorosty są tworzone losowo, dla każdego jest losowany obrót.
* Ryby są zaimplementowane obiektowo i posiadają dwie metody: `Move()` i `Render()`. Obie metody wywoływane są z poziomu `renderScene()`
* Ryba porusza się także po sin'usie góra/dół względem czasu: `temp.y -= 1.2*sinf(glutGet(GLUT_ELAPSED_TIME) / 500.0f - random);` gdzie random jest losową wartościa, generowaną podczas tworzenia obiektu Ryby
* Obiektem, którym sterujemy jest rekin, porusza się on: do przodu/tyłu, na boki, góra/dół oraz obraca prawo/lewo.
* W momencie zblizenia się rekina do ryby, ryba zwiększa swoją prędkość(ucieka)
* W programie jest zaimplementowany Skybox
* Ryba w momencie wypłynięcia poza skybox znika, a w jej miejsce pojawia się nowa w losowym punkcie.
* W shaderze jest zaimplementowane mapowanie normalnych
* Scena posiada 3 źródła światła

![Rybki](https://github.com/q12e/GRK_projekt/blob/master/Grk-plywajac%20z%20rybkami.png)
