import { FaLinux, FaWindows, FaApple } from "react-icons/fa"

export default function Download() {
  return (
    <main className="flex-1 bg-zinc-950 text-white px-4 sm:px-8 py-16">
      <section className="max-w-4xl mx-auto text-center">

        <h1 className="text-4xl sm:text-5xl font-bold tracking-tight">
          Download Maryn Launcher
        </h1>
        <p className="mt-4 text-zinc-400 text-lg">
          Choose your platform below.
        </p>

        <div className="mt-12 grid grid-cols-1 sm:grid-cols-3 gap-6">

          
           <a href="https://github.com/DinankaKoirala/Maryn_Launcher/releases/download/v0.2.0/Maryn-Launcher-Windows.zip"
            className="flex flex-col items-center gap-4 bg-zinc-900 border border-zinc-800 rounded-xl p-8 hover:border-blue-500 hover:bg-zinc-800 transition-colors"
          >
            <FaWindows className="text-5xl text-blue-400" />
            <span className="text-lg font-semibold">Windows</span>
            <span className="text-zinc-400 text-sm">v0.2.0 · .zip</span>
          </a>

          
            <a href="https://github.com/DinankaKoirala/Maryn_Launcher/releases/download/v0.2.0/Maryn_Launcher-x86_64.AppImage"
            className="flex flex-col items-center gap-4 bg-zinc-900 border border-zinc-800 rounded-xl p-8 hover:border-orange-500 hover:bg-zinc-800 transition-colors"
          >
            <FaLinux className="text-5xl text-orange-400" />
            <span className="text-lg font-semibold">Linux</span>
            <span className="text-zinc-400 text-sm">v0.2.0 · .AppImage</span>
          </a>

          
            <a href="https://github.com/DinankaKoirala/Maryn_Launcher/releases/download/v0.2.0/Maryn-Launcher-Mac.dmg"
            className="flex flex-col items-center gap-4 bg-zinc-900 border border-zinc-800 rounded-xl p-8 hover:border-zinc-400 hover:bg-zinc-800 transition-colors"
          >
            <FaApple className="text-5xl text-zinc-300" />
            <span className="text-lg font-semibold">macOS</span>
            <span className="text-zinc-400 text-sm">v0.2.0 · .dmg</span>
          </a>

        </div>

        <p className="mt-10 text-zinc-500 text-sm">
          All releases are available on{" "}
          
           <a href="https://github.com/DinankaKoirala/Maryn_Launcher/releases"
            target="_blank"
            rel="noopener noreferrer"
            className="text-zinc-300 hover:text-white underline transition-colors"
          >
            GitHub Releases
          </a>
          .
        </p>

      </section>
    </main>
  )
}