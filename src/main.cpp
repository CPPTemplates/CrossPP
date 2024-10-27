#include "application/initialize.h"
#include "application/control/form/form.h"
#include "include/application/application.h"
#include "gameInfo.h"
#include "math/random/random.h"
struct gameForm : public form
{
    virtual void render(cveci2 &position, const texture &renderTarget) override
    {
        for(color& ptr : renderTarget){
            ptr = color((byte)rand(0x100), (byte)rand(0x100), (byte)rand(0x100));
        }
        //renderTarget.fill(color((byte)rand(0x100), (byte)rand(0x100), (byte)rand(0x100)));
    }
};
gameForm *mainForm = new gameForm();
int main(int argc, char *argv[])
{
    // execute this function before you do anything,
    initialize();
    return application(mainForm, gameName).run();
}